# How to run a sample

## Linux
1) Build the engine
2) Set the `blitzpath` environment variable to the path of the _release directory in my case it would look like this
```bash
$ export blitzpath=/home/izarif/blitz3d-ng/_release
```
3) Find the main bb file of the desired sample
In my case, the file is located at '/home/izarif/blitz3d-ng/_release/samples/mak/driver/driver.bb'
4) Execute the following command
```bash
$ bin/blitz3d run <sample_main_file.bb>
```
where `<sample_main_file.bb>` is the path from the previous step
