
package net.homeip.ofn.androix;

public class AndroiXLib {
    public native void init();

    static
    {
        System.loadLibrary("androix");
    }
}
