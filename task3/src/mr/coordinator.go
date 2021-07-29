package mr

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"net/rpc"
	"os"
	"sort"
	"strconv"
	"sync"
	"time"
)

type Coordinator struct {
	// Your definitions here.
	MapDone          bool
	ReduceDone       bool
	AllDone          bool
	ReduceNumber     int
	File             []string
	MapIndex         int
	ReduceIndex      int
	MapDoneNumber    int
	ReduceDoneNumber int
	WorkersNumber    int
	MapResult        [10]ByKey
	ReduceResult     ByKey
	Exit             bool
	MapStartTime     [10000]time.Time
	MapBegin         bool
	MapStart         [10000]bool
	MapAllStart      bool
	MapFinish        [10000]bool
	ReduceStartTime  [10]time.Time
	ReduceBegin      bool
	ReduceStart      [10]bool
	ReduceAllStart   bool
	ReduceFinish     [10]bool
	mutex            sync.Mutex
}

// Your code here -- RPC handlers for the worker to call.
func (c *Coordinator) GetTask(args *Args, reply *Reply) error {
	if !c.MapAllStart {
		reply.TaskType = MapType
		c.mutex.Lock()
		for i := 0; i < len(c.File); i++ {
			if !c.MapStart[i] {
				c.MapIndex = i
				c.MapStart[i] = true
				c.MapStartTime[i] = time.Now()
				fmt.Println("map" + " " + strconv.Itoa(i) + " " + "start")
				break
			}
		}

		if c.MapIndex == 0 {
			c.MapBegin = true
		}
		reply.FileName = c.File[c.MapIndex]
		reply.TaskIndex = c.MapIndex
		reply.ReduceNumber = c.ReduceNumber
		if c.MapIndex == (len(c.File) - 1) {
			c.MapAllStart = true
		}
		c.mutex.Unlock()
		return nil
	} else if c.MapDone && !c.ReduceAllStart {
		reply.TaskType = ReduceType
		c.mutex.Lock()
		for i := 0; i < 10; i++ {
			if !c.ReduceStart[i] {
				c.ReduceIndex = i
				c.ReduceStart[c.ReduceIndex] = true
				c.ReduceStartTime[c.ReduceIndex] = time.Now()
				break
			}
		}

		fmt.Println("reduce" + " " + strconv.Itoa(c.ReduceIndex) + " " + "start")
		if c.ReduceIndex == 0 {
			c.ReduceBegin = true
		}
		reply.TaskIndex = c.ReduceIndex
		reply.ReduceSource = append(reply.ReduceSource, c.MapResult[reply.TaskIndex]...)

		if c.ReduceIndex == 9 {
			c.ReduceAllStart = true
		}
		c.mutex.Unlock()
		return nil
	} else if c.AllDone {
		reply.AllDone = true
		return nil
	} else {
		reply.Wait = true
		return nil
	}
}

func (c *Coordinator) TaskDone(args *Args, reply *Reply) error {
	if args.TaskType == MapType {
		if c.MapFinish[args.TaskIndex] {
			return nil
		}
		c.mutex.Lock()
		c.MapDoneNumber++
		for i := 0; i < 10; i++ {
			if len(args.MapResult[i]) == 0 {
				break
			}
			for j := 0; j < len(args.MapResult[i]); j++ {
				c.MapResult[i] = append(c.MapResult[i], args.MapResult[i][j])
			}
		}
		c.MapFinish[args.TaskIndex] = true
		fmt.Println("map" + " " + strconv.Itoa(args.TaskIndex) + " " + "finish")
		if c.MapDoneNumber == len(c.File) {
			c.MapDone = true
		}
		c.mutex.Unlock()
		return nil
	} else {
		if c.ReduceFinish[args.TaskIndex] {
			return nil
		}
		c.mutex.Lock()
		c.ReduceDoneNumber++
		for i := 0; i < args.ReduceResult.Len(); i++ {
			temp := args.ReduceResult[i]
			c.ReduceResult = append(c.ReduceResult, temp)
		}
		c.ReduceFinish[args.TaskIndex] = true
		fmt.Println("reduce" + " " + strconv.Itoa(args.TaskIndex) + " " + "finish")
		if c.ReduceDoneNumber == 10 {
			c.ReduceDone = true
			sort.Sort(ByKey(c.ReduceResult))
			oname := "mr-out-new"
			ofile, _ := os.Create(oname)
			for i := 0; i < c.ReduceResult.Len(); i++ {
				fmt.Fprintf(ofile, "%v %v\n", c.ReduceResult[i].Key, c.ReduceResult[i].Value)
			}
			ofile.Close()
			c.AllDone = true
			fmt.Println("task done! wait workers to exit")
		}
		c.mutex.Unlock()
		return nil
	}
}

func (c *Coordinator) WorkerNumber(args *Args, reply *Reply) error {
	if args.WorkerStart {
		c.mutex.Lock()
		c.WorkersNumber++
		c.mutex.Unlock()
		fmt.Println("worker" + " " + strconv.Itoa(c.WorkersNumber) + " " + "start")
		return nil
	} else {
		fmt.Println("worker" + " " + strconv.Itoa(c.WorkersNumber) + " " + "exit")
		c.mutex.Lock()
		c.WorkersNumber--
		c.mutex.Unlock()
		if c.WorkersNumber == 0 {
			fmt.Println("all workers exit,coordinator is already to exit")
			c.Exit = true
			return nil
		}
		return nil
	}
}

//
// start a thread that listens for RPCs from worker.go
//
func (c *Coordinator) server() {
	rpc.Register(c)
	rpc.HandleHTTP()
	//l, e := net.Listen("tcp", ":1234")
	sockname := coordinatorSock()
	os.Remove(sockname)
	l, e := net.Listen("unix", sockname)
	if e != nil {
		log.Fatal("listen error:", e)
	}
	go http.Serve(l, nil)
}

//
// main/mrcoordinator.go calls Done() periodically to find out
// if the entire job has finished.
//

//
// create a Coordinator.
// main/mrcoordinator.go calls this function.
// nReduce is the number of reduce tasks to use.
//
func MakeCoordinator(files []string, nReduce int) *Coordinator {
	c := Coordinator{}

	c.MapDone = false
	c.AllDone = false
	c.ReduceNumber = nReduce
	c.File = files
	c.MapDoneNumber = 0
	c.MapIndex = 0
	c.ReduceDone = false
	c.ReduceDoneNumber = 0
	c.ReduceIndex = 0
	c.WorkersNumber = 0
	c.MapAllStart = false
	c.MapBegin = false
	c.ReduceAllStart = false
	c.ReduceBegin = false
	c.Exit = false
	for i := 0; i < len(c.File); i++ {
		c.MapStart[i] = false
		c.MapFinish[i] = false
	}
	for i := 0; i < 10; i++ {
		c.ReduceStart[i] = false
		c.ReduceFinish[i] = false
	}
	c.server()
	return &c
}
