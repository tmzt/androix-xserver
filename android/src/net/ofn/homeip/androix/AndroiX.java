
package net.homeip.ofn.androix;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;

import android.widget.TextView;


public class AndroiX extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

//        TextView  tv = new TextView(this);
//        tv.setText( "no string from jni" );
//        setContentView(tv);

        final Intent intent = new Intent(this, AndroiXService.class);
        startService(intent);

        new Thread(new Runnable() {
            @Override
            public void run()
            {
                try { while(AndroiXService.blitView == null) Thread.sleep(250); } catch (InterruptedException e) {};
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        setContentView(AndroiXService.blitView);
                        AndroiXService.blitView.resume();
                        AndroiXService.blitView.invalidate();
                    }
                });
            }
        }, "AndroiX Setup View Thread").start();

    }

    @Override
    public void onPause()
    {
       Log.d("AndroiX", "paused");
       super.onPause();
       AndroiXService.blitView.suspend();
    }       

    @Override
    public void onResume()
    {
        Log.d("AndroiX", "resumed");
        super.onResume();
        if (AndroiXService.blitView != null) {
            try { setContentView(AndroiXService.blitView); }
            catch (IllegalStateException ex) {
                ViewGroup g = (ViewGroup)AndroiXService.blitView.getParent();
                g.removeView(AndroiXService.blitView);
                setContentView(AndroiXService.blitView);
            }
            AndroiXService.blitView.resume();
        }
    }

    @Override
    public void onDestroy()
    {
        Log.d("AndroiX", "destroyed");
    }

}
