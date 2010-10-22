
package net.homeip.ofn.androix;

import android.util.Log;

public class AndroiXDixMain implements Runnable
{
    public void run() {
          Log.i("AndroiX","in AndroiXDixMain java");
          AndroiXLib lib = new AndroiXLib();
          lib.init();
    }
}      
