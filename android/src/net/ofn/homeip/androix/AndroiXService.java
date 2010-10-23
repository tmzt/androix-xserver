
package net.homeip.ofn.androix;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
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

        lib = new AndroiXLib();

        AndroiXDixMain dixmain = new AndroiXDixMain();
        Thread mainthread = new Thread(dixmain, "AndroiX DIX Thread");
        mainthread.setDaemon(true);
        mainthread.start();

        instance = this;
    }

/* service should not restart when it dies */
/*
    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        return START_STICKY;
    }
*/

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }
}



