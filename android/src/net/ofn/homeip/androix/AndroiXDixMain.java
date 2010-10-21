
package net.homeip.ofn.androix;

import android.util.Log;

public class AndroiXDixMain implements Runnable
{
    public void run() {
          Log.i("AndroiX","in AndroiXDixMain java");
//          while(true) {Log.i("AndroiX","running");}
          AndroiXFakeLib fakelib = new AndroiXFakeLib();
          fakelib.init();
//        AndroiXFakeLib.init();
    }
}      
