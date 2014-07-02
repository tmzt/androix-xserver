
package net.homeip.ofn.androix;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.view.View;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import java.io.*;
import java.util.*;
import java.util.zip.*;

public class AndroiXService extends Service {

    public static AndroiXBlitView blitView;
    private static AndroiXService instance;
    protected static AndroiXLib lib;
    public static int screenWidth = 800;
    public static int screenHeight = 480;

    public static AndroiXService getService() { return instance; }
    
    @Override
    public void onCreate()
    {

        /* create the 2d view */
        blitView = new AndroiXBlitView(this);

        /* extract resources */
        try {
            BufferedOutputStream os;
            BufferedInputStream is;
            ZipEntry entry;

            File usrdata = new File("/data/data/net.homeip.ofn.androix/lib/libusrdata.so");
            ZipFile zip = new ZipFile(usrdata);
            Enumeration<? extends ZipEntry> e = zip.entries();
            File folder;
            while(e.hasMoreElements()) {
                entry = (ZipEntry)e.nextElement();
                Log.d("AndroiX", "extracting " + "/data/data/net.homeip.ofn.androix/" + entry.getName());
                if (entry.isDirectory()) {
                    folder = new File("/data/data/net.homeip.ofn.androix/" + entry.getName());
                    Log.d("AndroiX", "creating directory: " + "/data/data/net.homeip.ofn.androix/" + entry.getName());
                    try { folder.mkdirs(); } catch (SecurityException ex) {Log.d("AndroiX", "Could not create directory: " + entry.getName()); ex.printStackTrace(); }
                    continue;
                }
                is = new BufferedInputStream(zip.getInputStream(entry), 1024);
                FileOutputStream fos = new FileOutputStream("/data/data/net.homeip.ofn.androix/" + entry.getName());
                folder = new File(new File("/data/data/net.homeip.ofn.androix/" + entry.getName()).getParent());
                if (!folder.exists()) folder.mkdirs();
                os = new BufferedOutputStream(fos, 1024);
                byte buf[] = new byte[1024];
                int n;
                while ((n = is.read(buf, 0, 1024)) != -1) {
                    os.write(buf, 0, n);
                };
                os.flush();
                os.close();
                is.close();
            };
        } catch (Exception ex) { ex.printStackTrace(); Log.d("AndroiX", "failed to extract."); }

        Log.d("AndroiX", "Done extracting /usr");


        instance = this;
    }

    /* service should not restart when it dies */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
    	Log.d("AndroiX", "onStartCommand: screen size = (" + screenWidth + "x" + screenHeight + ")");
        lib = new AndroiXLib(screenWidth, screenHeight);

        Log.d("AndroiX", "Waiting for View");
        try { while(AndroiXService.blitView == null) Thread.sleep(250); } catch (InterruptedException e) {};
//      Log.d("AndroiX", "Waiting for mCreated");
//      try { while(!AndroiXService.blitView.getIsCreated()) Thread.sleep(250); } catch (InterruptedException e) {};
        Log.d("AndroiX", "Now starting the X server");

        Thread mainthread = new Thread(lib, "AndroiX DIX Thread");
        mainthread.setDaemon(true);
        mainthread.start();
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }
}



