
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

        TextView  tv = new TextView(this);
        tv.setText( "no string from jni" );
        setContentView(tv);

        final Intent intent = new Intent(this, AndroiXService.class);
        startService(intent);
    }

    @Override
    public void onDestroy()
    {
        Log.d("AndroiX", "destroyed");
    }

}
