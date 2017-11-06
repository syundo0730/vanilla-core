package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
)

func launchBlueTooth() {
	cmd := exec.Command("bash", "./tstblth.sh")
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	cmd.Start()

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		txt := scanner.Text()
		fmt.Println(txt)
		if txt == "5" {
			break;
		}
	}
	cmd.Wait()
}

func launchApp() {
	cmd := exec.Command("./Main")
	err := cmd.Start()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	cmd.Wait()
}

func main() {
	go launchBlueTooth()
	fmt.Println("hugaaa")
	for {}
}