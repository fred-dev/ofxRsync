//
//  ofxRsync.h
//  openframeworks
//
//  Created by Fred Rodrigues on 10/01/2023.
//

#ifndef ofxRsync_h
#define ofxRsync_h

#pragma once

#include "ofMain.h"
#include <atomic>


class ofxRsync : public ofThread {
public:
    // Event for notifications
    ofEvent<string> copyCompleteEvent;
    
    // Constructor
    ofxRsync() : running(false) {}
    
    ~ofxRsync(){
        stopThread();
        waitForThread(false);
    }
    
    // Thread function
    void threadedFunction(){
        Item item;
        while(isThreadRunning()) {
            // Check if there are any items in the queue
            if (queue.tryReceive(item)) {
                // Set up the command to be executed
                string command = "rsync ";
                if (item.recursive) command += "-r ";
                if (item.deleteExtra) command += "--delete ";
                command += item.src + " " + item.user + "@" + item.host + ":" + item.dst;
                
                // Execute the command using `system()`
                string result = ofSystem(command);
                ofLog(OF_LOG_VERBOSE, "ofxRsync::threadedFunction Result of command: " + ofToString(result));

                // Notify the main thread that the copy is complete
                ofNotifyEvent(copyCompleteEvent, item.src, this);
            } else {
                // If the queue is empty, stop the thread
                stopThread();
                running = false;
            }
        }
    }
    
    void send(string host, string user, string src, string dst, bool recursive = false, bool deleteExtra = false){
        Item item;
        item.host = host;
        item.user = user;
        item.src = src;
        item.dst = dst;
        item.recursive = recursive;
        item.deleteExtra = deleteExtra;
        queue.send(item);
        
        // If the thread is not running, start it
        if (!running) {
            startThread();
            running = true;
        }
    }

private:
    // Structure for storing items in the queue
    struct Item {
        string host;        // Hostname or IP address of the remote machine
        string user;        // Username for the remote machine
        string src;         // Source file or directory to be copied
        string dst;         // Destination directory on the remote machine
        bool recursive;    // Flag indicating whether to copy directories recursively
        bool deleteExtra;  // Flag indicating whether to delete extra files in the destination directory
    };
    
    ofThreadChannel<Item> queue;    // Thread channel for the copy queue
    std::atomic<bool> running;        // Flag indicating whether the thread is running
};

#endif /* ofxRsync_h */
