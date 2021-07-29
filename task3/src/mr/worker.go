package mr

import (
	"fmt"
	"hash/fnv"
	"io/ioutil"
	"log"
	"net/rpc"
	"os"
	"sort"
	"strconv"
	"time"
)

//
// Map functions return a slice of KeyValue.
//
type KeyValue struct {
	Key   string
	Value string
}
type ByKey []KeyValue

func (a ByKey) Len() int           { return len(a) }
func (a ByKey) Swap(i, j int)      { a[i], a[j] = a[j], a[i] }
func (a ByKey) Less(i, j int) bool { return a[i].Key < a[j].Key }

//
// use ihash(key) % NReduce to choose the reduce
// task number for each KeyValue emitted by Map.
//
func ihash(key string) int {
	h := fnv.New32a()
	h.Write([]byte(key))
	return int(h.Sum32() & 0x7fffffff)
}

//
// main/mrworker.go calls this function.
//
func Worker(mapf func(string, string) []KeyValue, reducef func(string, []string) string) {

	// Your worker implementation here.
	WorkerStart()
	reply := Reply{}
	reply.AllDone = false
	reply.ReduceNumber = 10
	reply.TaskIndex = 0
	reply.TaskType = -1
	reply.Wait = false
	for {
		reply = CallCoordinator()
		if reply.Wait {
			continue
		}
		if reply.AllDone {
			fmt.Println("already to exit")
			WorkerExit()
			time.Sleep(time.Second)
			os.Exit(0)
		}
		if reply.TaskType == MapType {
			fmt.Println("get map task" + strconv.Itoa(reply.TaskIndex))
			time.Sleep(2 * time.Second)
			intermediate := []KeyValue{}
			filename := reply.FileName
			file, err := os.Open(filename)
			if err != nil {
				log.Fatalf("cannot open %v", filename)
			}
			content, err := ioutil.ReadAll(file)
			if err != nil {
				log.Fatalf("cannot read %v", filename)
			}
			file.Close()
			kva := mapf(filename, string(content))
			intermediate = append(intermediate, kva...)
			sort.Sort(ByKey(intermediate))
			maptoreduce := [10]ByKey{}
			for i := 0; i < len(intermediate); i++ {
				reduce_index := ihash(intermediate[i].Key) % 10
				temp := intermediate[i]
				maptoreduce[reduce_index] = append(maptoreduce[reduce_index], temp)
			}
			fmt.Println("finish map task" + strconv.Itoa(reply.TaskIndex))
			ReportToCoordinatorForMap(reply, maptoreduce)
		} else if reply.TaskType == ReduceType {
			fmt.Println("get reduce task" + strconv.Itoa(reply.TaskIndex))
			time.Sleep(2 * time.Second)
			sort.Sort(ByKey(reply.ReduceSource))
			reduceresult := []KeyValue{}
			for i := 0; i < len(reply.ReduceSource); {
				j := i + 1
				for j < len(reply.ReduceSource) && reply.ReduceSource[j].Key == reply.ReduceSource[i].Key {
					j++
				}
				values := []string{}
				for k := i; k < j; k++ {
					temp := reply.ReduceSource[k].Value
					values = append(values, temp)
				}
				output := reducef(reply.ReduceSource[i].Key, values)
				reduce_result_of_one_key := KeyValue{reply.ReduceSource[i].Key, output}
				reduceresult = append(reduceresult, reduce_result_of_one_key)
				i = j
			}
			fmt.Println("finish reduce task" + strconv.Itoa(reply.TaskIndex))
			ReportToCoordinatorForReduce(reply, reduceresult)
		} else {
			fmt.Println("failed to get task")
		}
		time.Sleep(5 * time.Second)
	}
	// uncomment to send the Example RPC to the coordinator.
	// CallExample()
}

//
// example function to show how to make an RPC call to the coordinator.
//
// the RPC argument and reply types are defined in rpc.go.
//

func CallCoordinator() (reply Reply) {
	args := Args{}
	reply = Reply{}
	error := call("Coordinator.GetTask", &args, &reply)
	if !error {
		reply.TaskType = -1
	}
	return reply
}

func ReportToCoordinatorForMap(arg Reply, mapresult [10]ByKey) {
	args := Args{}
	reply := Reply{}
	args.TaskType = arg.TaskType
	args.TaskIndex = arg.TaskIndex
	for i := 0; i < 10; i++ {
		if len(mapresult[i]) == 0 {
			break
		}
		for j := 0; j < len(mapresult[i]); j++ {
			args.MapResult[i] = append(args.MapResult[i], mapresult[i][j])
		}
	}
	call("Coordinator.TaskDone", &args, &reply)
}

func ReportToCoordinatorForReduce(arg Reply, reduceresult ByKey) {
	args := Args{}
	reply := Reply{}
	args.TaskType = arg.TaskType
	args.TaskIndex = arg.TaskIndex
	args.ReduceResult = reduceresult
	call("Coordinator.TaskDone", &args, &reply)
}

func WorkerStart() {
	args := Args{}
	reply := Reply{}
	args.WorkerStart = true
	call("Coordinator.WorkerNumber", &args, &reply)
}

func WorkerExit() {
	args := Args{}
	reply := Reply{}
	args.WorkerStart = false
	call("Coordinator.WorkerNumber", &args, &reply)
}

//
// send an RPC request to the coordinator, wait for the response.
// usually returns true.
// returns false if something goes wrong.
//
func call(rpcname string, args interface{}, reply interface{}) bool {
	// c, err := rpc.DialHTTP("tcp", "127.0.0.1"+":1234")
	sockname := coordinatorSock()
	c, err := rpc.DialHTTP("unix", sockname)
	if err != nil {
		log.Fatal("dialing:", err)
	}
	defer c.Close()

	err = c.Call(rpcname, args, reply)
	if err == nil {
		return true
	}

	fmt.Println(err)
	return false
}
