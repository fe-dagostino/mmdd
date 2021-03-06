
<a href="https://travis-ci.org/">
  <img alt="Travis-CI Build Status"
       src="https://travis-ci.org/fe-dagostino/mmdd.svg?branch=master"/>
</a>

Make Me Discoverable Daemon (MMDD)
====

It make easy to find out devices connected over the same network.      
Application has been divided in three different parts

1. mmdd : that's the damemon to be installed on your devices. Will be possible 
          to configure a serial number and a description for each device in order   
          to simplify device identification.  
2. mmdc : this is a command line client, it just send a broadcast message and 
          diplay all replies to the users. Each reply will contains some information
          like source ip, serial number and description.
3. mmde : if mmdd has been compiled with MMDD_ENABLE_EXECUTION will be possible to
          use this command line tool in order to execute commands on the specific
          devices or on all devices at the same time. It is useful if you need, for 
          example, to reboot all devices in your network without to connect to each 
          device and to reboot it. This feature can helps too much but at the moment
          can be really dangerous if don't have the control of each devices in your 
          network.    


How to build 
----

**FEDLibrary** - *http://sourceforge.net/projects/fedlibrary/*
```
git clone git://git.code.sf.net/p/fedlibrary/git fedlibrary
cd fedlibrary 
mkdir build 
cd build
cmake ../ -DFORCE_PIC=ON 
make 
make install 
ldconfig 
cd ../../ 
```

**MMDD** 
```
git clone https://github.com/fe-dagostino/mmdd.git mmdd
cd mmdd
mkdir build
cd build
cmake ../
make
```

