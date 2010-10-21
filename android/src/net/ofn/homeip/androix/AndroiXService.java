
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

public class AndroiXService extends Service {

    @Override
    public void onCreate()
    {
        AndroiXDixMain dixmain = new AndroiXDixMain();
        Thread mainthread = new Thread(dixmain, "AndroiX DIX Thread");
        mainthread.setDaemon(true);
        mainthread.start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }
}



