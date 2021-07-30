package mr

import (
	"log"
	"net"
	"net/http"
	"net/rpc"
	"os"
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
	if !c.MapAllStart && !c.MapDone {
		reply.TaskType = MapType
		c.mutex.Lock()
		for i := 0; i < len(c.File); i++ {
			if !c.MapStart[i] {
				c.MapIndex = i
				c.MapStart[i] = true
				c.MapStartTime[i] = time.Now()
				//fmt.Println("map" + " " + strconv.Itoa(i) + " " + "start")
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

		//fmt.Println("reduce" + " " + strconv.Itoa(c.ReduceIndex) + " " + "start")
		if c.ReduceIndex == 0 {
			c.ReduceBegin = true
		}
		reply.TaskIndex = c.ReduceIndex
		reply.FileNumber = len(c.File)
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
		c.mutex.Lock()
		if c.MapFinish[args.TaskIndex] {
			c.mutex.Unlock()
			return nil
		}
		c.MapDoneNumber++
		c.MapFinish[args.TaskIndex] = true
		//fmt.Println(" map task " + strconv.Itoa(args.TaskIndex) + "finish")
		//fmt.Println("map" + " " + strconv.Itoa(args.TaskIndex) + " " + "finish")
		if c.MapDoneNumber == len(c.File) {
			c.MapDone = true
		}
		c.mutex.Unlock()
		return nil
	} else if args.TaskType == ReduceType {
		c.mutex.Lock()
		if c.ReduceFinish[args.TaskIndex] {
			c.mutex.Unlock()
			return nil
		}
		c.ReduceDoneNumber++
		c.ReduceFinish[args.TaskIndex] = true
		//fmt.Println(" reduce task " + strconv.Itoa(args.TaskIndex) + "finish")
		//fmt.Println("reduce" + " " + strconv.Itoa(args.TaskIndex) + " " + "finish")
		if c.ReduceDoneNumber == 10 {
			c.ReduceDone = true
			c.AllDone = true
			//fmt.Println("task done! wait workers to exit")
		}
		c.mutex.Unlock()
		return nil
	} else {
		return nil
	}
}

func (m *Coordinator) Obversor() {
	for {
		if m.MapBegin {
			m.mutex.Lock()
			for i := 0; i < len(m.File); i++ {
				if m.MapStart[i] && !m.MapFinish[i] && (time.Now().After(m.MapStartTime[i].Add(5 * time.Second))) {
					m.MapStart[i] = false
					m.MapAllStart = false
				}
			}
			m.mutex.Unlock()
		}
		if m.ReduceBegin {
			m.mutex.Lock()
			for i := 0; i < 10; i++ {
				if m.ReduceStart[i] && !m.ReduceFinish[i] && (time.Now().After(m.ReduceStartTime[i].Add(5 * time.Second))) {
					m.ReduceStart[i] = false
					m.ReduceAllStart = false
				}
			}
			m.mutex.Unlock()
		}
	}
}

func (c *Coordinator) Done() bool {
	ret := false

	// Your code here.
	if c.AllDone {
		ret = true
	}

	return ret
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
	go c.Obversor()
	c.server()
	return &c
}
