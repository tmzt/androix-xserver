
package net.homeip.ofn.androix;

public class AndroiXLib {
    public native void init();
    public native void keyDown(int kbd, int keyCode);
    public native void keyUp(int kbd, int keyCode);
    public native void touchDown(int mouse, int x, int y);
    public native void touchUp(int mouse, int x, int y);

    static
    {
        System.loadLibrary("androix");
    }
}
