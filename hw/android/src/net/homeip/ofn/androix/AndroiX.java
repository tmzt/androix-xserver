
package net.homeip.ofn.androix;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.Display;

import android.widget.TextView;


public class AndroiX extends Activity
{
    private static AndroiX instance = null;
    public static AndroiX getActivity() { return instance; }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        Display display = getWindowManager().getDefaultDisplay(); 
        AndroiXService.screenWidth = display.getWidth();
        AndroiXService.screenHeight = display.getHeight();
        final Intent intent = new Intent(this, AndroiXService.class);
        startService(intent);

        /*
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
        */

//      Log.d("AndroiX", "Waiting for View in Activity");
//      try { while(AndroiXService.blitView == null) Thread.sleep(250); } catch (InterruptedException e) {};
//      Log.d("AndroiX", "Setting the View");
//        setContentView(AndroiXService.blitView); 

        instance = this;
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
        instance = null;
        super.onDestroy();
        Log.d("AndroiX", "destroyed");
    }

}
