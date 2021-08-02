package mr

//
// RPC definitions.
//
// remember to capitalize all names.
//

import "os"
import "strconv"
import "fmt"
import "time"

//
// example to show how to declare the arguments
// and reply for an RPC.
//

const (
	END=0;
	MAP=1;
	REDUCE=2;
)

type Task struct{
	Type int	//enum above
	Index int
	InputFile string
	WorkerName string
	ddl time.Time
}

type TaskArgv struct {
	WorkerName string
	LastType int
	LastIndex int
}

type TaskReply struct{
	TaskType int
	TaskIndex int
	InputFile string
	nMap int
	nReduce int
}

func tmpMapOutFile(worker string, mapIndex int, reduceIndex int) string {
	return fmt.Sprintf("tmp-worker-%s-%d-%d", worker, mapIndex, reduceIndex)
}

func finalMapOutFile(mapIndex int, reduceIndex int) string {
	return fmt.Sprintf("mr-%d-%d", mapIndex, reduceIndex)
}

func tmpReduceOutFile(worker string, reduceIndex int) string {
	return fmt.Sprintf("tmp-worker-%s-out-%d", worker, reduceIndex)
}

func finalReduceOutFile(reduceIndex int) string {
	return fmt.Sprintf("mr-out-%d", reduceIndex)
}

func GenTaskID(t int, index int) string {
	return fmt.Sprintf("%d-%d", t, index)
}

type ExampleArgs struct {
	X int
}

type ExampleReply struct {
	Y int
}

// Add your RPC definitions here.


// Cook up a unique-ish UNIX-domain socket name
// in /var/tmp, for the coordinator.
// Can't use the current directory since
// Athena AFS doesn't support UNIX-domain sockets.
func coordinatorSock() string {
	s := "/var/tmp/824-mr-"
	s += strconv.Itoa(os.Getuid())
	return s
}
