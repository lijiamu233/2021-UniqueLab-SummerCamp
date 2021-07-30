package mr

//
// RPC definitions.
//
// remember to capitalize all names.
//

import (
	"os"
	"strconv"
)

//
// example to show how to declare the arguments
// and reply for an RPC.
//

// Add your RPC definitions here.
type Args struct {
	TaskType     int
	TaskIndex    int
	WorkerStart  bool
	MapResult    [10]ByKey
	ReduceResult ByKey
}
type Reply struct {
	TaskType     int
	TaskIndex    int
	FileName     string
	ReduceNumber int
	AllDone      bool
	ReduceSource ByKey
	Wait         bool
	FileNumber   int
}

const (
	MapType    = 0
	ReduceType = 1
)

// Cook up a unique-ish UNIX-domain socket name
// in /var/tmp, for the coordinator.
// Can't use the current directory since
// Athena AFS doesn't support UNIX-domain sockets.
func coordinatorSock() string {
	s := "/var/tmp/824-mr-"
	s += strconv.Itoa(os.Getuid())
	return s
}
