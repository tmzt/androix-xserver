
package net.homeip.ofn.androix;

public class AndroiXFakeLib {
    public native void init();

    static
    {
        System.loadLibrary("androix");
    }
}
