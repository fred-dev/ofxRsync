# ofxRsync

Header-only openFrameworks addon that syncs files or folders to a remote machine with `rsync`, on a background thread. Optional recursive copy and `--delete` for extra files at the destination. Fires `copyCompleteEvent` when it finishes.

```cpp
ofxRsync rsync;
ofAddListener(rsync.copyCompleteEvent, this, &ofApp::onSynced);
rsync.send("192.168.1.10", "fred", ofToDataPath("renders/"), "/home/fred/renders", true, false);
```

Needs `rsync` and `ssh` on the path and key-based login to the remote. See also [ofxSCP](https://github.com/fred-dev/ofxSCP).
