# Periodic : cron clone

`period` is a job scheduler. Users can run periodically scripts.

Scheduling is done from the `periodic` CLI tool *(Système - L2, S4)*.

## How to use it

 - Download/Clone the repository
 - Build `period` and `periodic` with `make`
 - Launch `period` as a *daemon* with `bin/launch_daemon ${PWD}/bin/period
 - Schedule jobs

## Features

With `periodic`, you can:
 - **Add a new command**: `bin/periodic START FREQUENCY COMMAND`
 - **List all commands**:`bin/periodic`
 - **Remove a command**: `bin/periodic COMMAND_ID`

About *START* and *FREQUENCY*:
 - *START*: `now`, `+X` (in X seconds) or `X` (EPOCH)
 - *FREQUENCY*: number of seconds