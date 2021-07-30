package mr

import (
	"encoding/json"
	"fmt"
	"hash/fnv"
	"io/ioutil"
	"log"
	"net/rpc"
	"os"
	"sort"
	"strconv"
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
			os.Exit(0)
		}
		if reply.TaskType == MapType {
			//fmt.Println("get map task" + strconv.Itoa(reply.TaskIndex))
			//time.Sleep(2 * time.Second)
			//fmt.Println("get map task " + strconv.Itoa(reply.TaskIndex))
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
			outprefix := "mr-"
			outprefix += strconv.Itoa(reply.TaskIndex)
			outprefix += "-"

			map_result := make([]ByKey, reply.ReduceNumber)
			for _, kv := range intermediate {
				outindex := ihash(kv.Key) % reply.ReduceNumber
				map_result[outindex] = append(map_result[outindex], kv)
			}
			for i := 0; i < reply.ReduceNumber; i++ {
				outname := outprefix + strconv.Itoa(i)
				newfile, _ := os.Create(outname)
				enc := json.NewEncoder(newfile)
				for _, kv := range map_result[i] {
					enc.Encode(&kv)
				}
				newfile.Close()
			}
			//fmt.Println("finish map task" + strconv.Itoa(reply.TaskIndex))
			ReportToCoordinatorForMap(reply)
		} else if reply.TaskType == ReduceType {
			//fmt.Println("get reduce task" + strconv.Itoa(reply.TaskIndex))
			//time.Sleep(2 * time.Second)
			//fmt.Println("get reduce task " + strconv.Itoa(reply.TaskIndex))
			outname := "mr-out-" + strconv.Itoa(reply.TaskIndex)
			innameprefix := "mr-"
			innamesuffix := "-" + strconv.Itoa(reply.TaskIndex)
			intermediate := []KeyValue{}
			for index := 0; index < reply.FileNumber; index++ {
				inname := innameprefix + strconv.Itoa(index) + innamesuffix
				file, err := os.Open(inname)
				if err != nil {
					fmt.Printf("Open intermediate file %v failed: %v\n", inname, err)
					panic("Open file error")
				}
				dec := json.NewDecoder(file)
				for {
					var kv KeyValue
					if err := dec.Decode(&kv); err != nil {
						//fmt.Printf("%v\n", err)
						break
					}
					//fmt.Printf("%v\n", kv)
					intermediate = append(intermediate, kv)
				}
				file.Close()
			}
			sort.Sort(ByKey(intermediate))
			ofile, err := os.Create(outname)
			if err != nil {
				fmt.Printf("Create output file %v failed: %v\n", outname, err)
				panic("Create file error")
			}
			i := 0
			for i < len(intermediate) {
				j := i + 1
				for j < len(intermediate) && intermediate[j].Key == intermediate[i].Key {
					j++
				}
				values := []string{}
				for k := i; k < j; k++ {
					values = append(values, intermediate[k].Value)
				}
				output := reducef(intermediate[i].Key, values)

				// this is the correct format for each line of Reduce output.
				fmt.Fprintf(ofile, "%v %v\n", intermediate[i].Key, output)

				i = j
			}
			ofile.Close()
			//fmt.Println("finish reduce task" + strconv.Itoa(reply.TaskIndex))
			ReportToCoordinatorForReduce(reply)
		} else {
			fmt.Println("failed to get task")
		}
		//time.Sleep(5 * time.Second)
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

func ReportToCoordinatorForMap(arg Reply) {
	args := Args{}
	reply := Reply{}
	args.TaskType = arg.TaskType
	args.TaskIndex = arg.TaskIndex

	call("Coordinator.TaskDone", &args, &reply)
}

func ReportToCoordinatorForReduce(arg Reply) {
	args := Args{}
	reply := Reply{}
	args.TaskType = arg.TaskType
	args.TaskIndex = arg.TaskIndex
	call("Coordinator.TaskDone", &args, &reply)
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
