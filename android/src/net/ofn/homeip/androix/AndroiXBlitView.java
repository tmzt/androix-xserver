
package net.homeip.ofn.androix;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.view.*;
import android.graphics.Bitmap;
import android.graphics.Canvas;

import android.util.Log;

import java.nio.*;

/* 2d version */

public class AndroiXBlitView extends View implements View.OnKeyListener {
    private int mScreenPtr = 0;
    private int mKeyboardPtr = 0;
    private Bitmap mBitmap = null;
    private ByteBuffer mBuf = null;
    private boolean mCreated = false;
    private boolean mDrawing = false;

    public AndroiXBlitView(Context context) {
        super(context);

        setOnKeyListener(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();

        int W = 800;
        int H = 480;

        //mBitmap = Bitmap.createBitmap(W, H, Bitmap.Config.RGB_565);
    }

    public boolean getIsDrawing() { return mDrawing; }
    public boolean getIsCreated() { return mCreated; }

    public int initNativeScreen(int screen) {
        mScreenPtr = screen;   /* only on 32bit */
        Log.d("AndroiX", "[blitView] initNativeScreen: screen: " + screen);
        return 0;
    }

    public int initNativeKeyboard(int kbd) {
        mKeyboardPtr = kbd;   /* only on 32bit */
        Log.d("AndroiX", "[blitView] initNativeKeyboard: kbd: " + kbd);
        return 0;
    }

    public int initFramebuffer(int width, int height, int depth, java.nio.ByteBuffer buf) {
        Log.d("AndroiX", "Initialize Framebuffer: " + width + " x " + height + " depth: " + depth);
        if (depth != 16) {
            Log.d("AndroiX", "Bad depth: " + depth);
            return -1;
        }
        mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);

        AndroiX.getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AndroiX.getActivity().setContentView(AndroiXService.blitView);
                setOnKeyListener(AndroiXService.blitView);
                AndroiXService.blitView.setFocusable(true);
                AndroiXService.blitView.setFocusableInTouchMode(true);
                AndroiXService.blitView.requestFocus();
                AndroiXService.blitView.resume();
                AndroiXService.blitView.invalidate();
            }
        });

//      AndroiX.getActivity().setContentView(this);

        mBuf = buf;
        return 0;
    }

    public void draw(int x, int y, int w, int h) {
        Log.d("AndroiX", "Draw from native: " + x + "," + y + "(" + w + " x " + h + ")");
        if (!mDrawing) {
            Log.d("AndroiX", "draw ignored while suspended");
            return;
        }
        mBitmap.copyPixelsFromBuffer(mBuf);
        postInvalidate();
    }

    public void suspend() {
        mDrawing = false;
    }

    public void resume() {
        mDrawing = true;
    }

    @Override protected void onDraw(Canvas canvas) {
        // assume we're created
        mCreated = true;

        if (mBitmap == null) {
            Log.d("AndroiX", "mBitmap not ready, did [native] run?");
            postInvalidate(); // call us again
        }

        canvas.drawBitmap(mBitmap, 0, 0, null);
        invalidate();
    }

    /* OnKeyListener */

    public boolean onKey(View v, int keyCode, KeyEvent event) {
        switch (event.getAction()) {
            case KeyEvent.ACTION_DOWN:
                Log.d("AndroiX", "onKey: ACTION_DOWN keyCode: " + keyCode);
                AndroiXService.lib.keyDown(mKeyboardPtr, keyCode);
                return true;
            case KeyEvent.ACTION_UP:
                Log.d("AndroiX", "onKey: ACTION_UP keyCode: " + keyCode);
                AndroiXService.lib.keyUp(mKeyboardPtr, keyCode);
                return true;
            /* not handling multiple keypresses yet */
        };
        return false;
    }
}

