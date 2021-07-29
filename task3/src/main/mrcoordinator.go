package main

//
// start the coordinator process, which is implemented
// in ../mr/coordinator.go
//
// go run mrcoordinator.go pg*.txt
//
// Please do not change this file.
//

import (
	"fmt"
	"os"
	"time"

	"6.824/mr"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, "Usage: mrcoordinator inputfiles...\n")
		os.Exit(1)
	}

	m := mr.MakeCoordinator(os.Args[1:], 10)
	for {
		if m.MapBegin {
			for i := 0; i < len(m.File); i++ {
				if m.MapStart[i] && !m.MapFinish[i] && (time.Now().After(m.MapStartTime[i].Add(4 * time.Second))) {
					m.MapStart[i] = false
					m.MapAllStart = false
					m.WorkersNumber--
				}
			}
		}
		if m.ReduceBegin {
			for i := 0; i < 10; i++ {
				if m.ReduceStart[i] && !m.ReduceFinish[i] && (time.Now().After(m.ReduceStartTime[i].Add(4 * time.Second))) {
					m.ReduceStart[i] = false
					m.ReduceAllStart = false
					m.WorkersNumber--
				}
			}
		}
		if m.Exit {
			time.Sleep(time.Second)
			os.Exit(0)
		}
		time.Sleep(time.Second)
	}

}
