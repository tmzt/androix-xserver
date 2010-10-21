
package net.homeip.ofn.androix;

public class AndroiXFakeLib {
    public static native void init();

    static
    {
        System.loadLibrary("androix");
    }
}
