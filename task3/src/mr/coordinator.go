package mr

import "log"
import "net"
import "os"
import "net/rpc"
import "net/http"
import "sync"
import "time"

type Coordinator struct {
	// Your definitions here.
	Tasks     map[string]Task
	nMap      int
	nReduce   int
	Statue    int //enum :0,1,2
	available chan Task

	lock sync.Mutex
}

// Your code here -- RPC handlers for the worker to call.

func (c *Coordinator) ChangeStatue() {
	if c.Statue == MAP {
		for i := 0; i < c.nReduce; i++ {
			tmp := Task{
				Type:  REDUCE,
				Index: i,
			}
			c.Tasks[GenTaskID(tmp.Type, tmp.Index)] = tmp
			c.available <- tmp
		}
	} else if c.Statue == REDUCE {
		log.Printf("All works done")
		close(c.available)
		c.Statue = 0
	}
}

func (c *Coordinator) ApplyForTask(args *TaskArgv, reply *TaskReply) error {
	if args.LastType != 0 {
		c.lock.Lock()
		lastTaskID := GenTaskID(args.LastType, args.LastIndex)
		if task, exists := c.Tasks[lastTaskID]; exists && task.WorkerName == args.WorkerName {
			log.Printf(
				"Mark %d task %d as finished on worker %s\n",
				task.Type, task.Index, args.WorkerName)
			if args.LastType == MAP {
				for ri := 0; ri < c.nReduce; ri++ {
					err := os.Rename(
						tmpMapOutFile(args.WorkerName, args.LastIndex, ri),
						finalMapOutFile(args.LastIndex, ri))
					if err != nil {
						log.Fatalf(
							"Failed to mark map output file `%s` as final: %e",
							tmpMapOutFile(args.WorkerName, args.LastIndex, ri), err)
					}
				}
			} else if args.LastType == REDUCE {
				err := os.Rename(
					tmpReduceOutFile(args.WorkerName, args.LastIndex),
					finalReduceOutFile(args.LastIndex))
				if err != nil {
					log.Fatalf(
						"Failed to mark reduce output file `%s` as final: %e",
						tmpReduceOutFile(args.WorkerName, args.LastIndex), err)
				}
			}
			delete(c.Tasks, lastTaskID)
			if len(c.Tasks) == 0 {
				c.ChangeStatue()
			}
		}
		c.lock.Unlock()
	}
	task, ok := <-c.available
	if !ok {
		return nil
	}

	c.lock.Lock()
	defer c.lock.Unlock()
	log.Printf("Assign %d task %d to worker %s\n", task.Type, task.Index, args.WorkerName)
	task.WorkerName = args.WorkerName
	task.ddl = time.Now().Add(10 * time.Second)
	c.Tasks[GenTaskID(task.Type, task.Index)] = task
	reply.TaskType = task.Type
	reply.TaskIndex = task.Index
	reply.InputFile = task.InputFile
	reply.nMap = c.nMap
	reply.nReduce = c.nReduce
	return nil
}

//
// an example RPC handler.
//
// the RPC argument and reply types are defined in rpc.go.
//
func (c *Coordinator) Example(args *ExampleArgs, reply *ExampleReply) error {
	reply.Y = args.X + 1
	return nil
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
func (c *Coordinator) Done() bool {

	// Your code here.

	return c.Statue == 0
}

//
// create a Coordinator.
// main/mrcoordinator.go calls this function.
// nReduce is the number of reduce tasks to use.
//
func MakeCoordinator(files []string, nReduce int) *Coordinator {
	c := Coordinator{
		Statue:    MAP,
		nMap:      len(files),
		nReduce:   nReduce,
		Tasks:     make(map[string]Task),
		available: make(chan Task),
	}

	// Your code here.
	for index, file := range files {
		tmp := Task{
			Type: MAP,
			Index: index,
			InputFile: file,
		}
		c.Tasks[GenTaskID(tmp.Type,tmp.Index)]=tmp
		c.available <- tmp
	}

	c.server()
	return &c
}
