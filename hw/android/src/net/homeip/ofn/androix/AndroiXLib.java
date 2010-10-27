
package net.homeip.ofn.androix;

import android.util.Log;

public class AndroiXLib implements Runnable {
    public native void init();
    public native void keyDown(int kbd, int keyCode);
    public native void keyUp(int kbd, int keyCode);
    public native void touchDown(int mouse, int x, int y);
    public native void touchUp(int mouse, int x, int y);

    public void run() {
          Log.i("AndroiX","in AndroiXLib main");
          init();
    }

    static
    {
        System.loadLibrary("androix");
    }
}
