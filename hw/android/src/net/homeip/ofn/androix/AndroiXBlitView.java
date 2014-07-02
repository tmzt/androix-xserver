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

public class AndroiXBlitView extends View implements View.OnKeyListener, View.OnTouchListener {
    private int mScreenPtr = 0;
    private int mKeyboardPtr = 0;
    private int mMousePtr = 0;
    private int mTrackballPtr = 0;
    private Bitmap mBitmap = null;
    private ByteBuffer mBuf = null;
    private boolean mCreated = false;
    private boolean mDrawing = false;

    class UiThread implements Runnable {
    	public void run() {
            AndroiX.getActivity().setContentView(AndroiXService.blitView);
            AndroiXService.blitView.setOnKeyListener(AndroiXService.blitView);
            AndroiXService.blitView.setOnTouchListener(AndroiXService.blitView);
            AndroiXService.blitView.setFocusable(true);
            AndroiXService.blitView.setFocusableInTouchMode(true);
            AndroiXService.blitView.requestFocus();
            AndroiXService.blitView.resume();
            AndroiXService.blitView.invalidate();
    	}
    }
    
    public AndroiXBlitView(Context context) {
        super(context);

        setOnKeyListener(this);
        setOnTouchListener(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();

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

    public int initNativeMouse(int mouse) {
        mMousePtr = mouse;   /* only on 32bit */
        Log.d("AndroiX", "[blitView] initNativeMouse: mouse: " + mouse);
        return 0;
    }

    public int initNativeTrackball(int ball) {
        mTrackballPtr = ball;   /* only on 32bit */
        Log.d("AndroiX", "[blitView] initNativeTrackball: ball: " + ball);
        return 0;
    }

    public int initFramebuffer(int width, int height, int depth, java.nio.ByteBuffer buf) {
        Log.d("AndroiX", "Initialize Framebuffer: " + width + " x " + height + " depth: " + depth);
        if (depth != 16) {
            Log.d("AndroiX", "Bad depth: " + depth);
            return -1;
        }
        mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);

        AndroiX.getActivity().runOnUiThread(new UiThread());

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
        //draw(0, 0, getWidth(), getHeight());
        draw(0, 0, 1280, 900);
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
        if (mKeyboardPtr == 0) { Log.d("AndroiX", "keyboard not ready. kbd: " + mKeyboardPtr); return false; }
        switch (event.getAction()) {
            case KeyEvent.ACTION_DOWN:
                Log.d("AndroiX", "onKey: ACTION_DOWN keyCode: " + keyCode);
                if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    AndroiXService.lib.trackballPress(mTrackballPtr);
                    AndroiXService.lib.keyDown(mKeyboardPtr, keyCode);
                } else if(keyCode == KeyEvent.KEYCODE_BACK) {
                	AndroiX.getActivity().finish();
                	return true;
                } else {
                    AndroiXService.lib.keyDown(mKeyboardPtr, keyCode);
                };
                return true;
            case KeyEvent.ACTION_UP:
                Log.d("AndroiX", "onKey: ACTION_UP keyCode: " + keyCode);
                if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) {
                    AndroiXService.lib.trackballRelease(mTrackballPtr);
                    AndroiXService.lib.keyUp(mKeyboardPtr, keyCode);

                } else {
                    AndroiXService.lib.keyUp(mKeyboardPtr, keyCode);
                };
                return true;
            	
            /* not handling multiple keypresses yet */
        };
        return false;
    }

    /* OnTouchListener */

    public boolean onTouch(View v, MotionEvent event) {
        if (mMousePtr == 0) { Log.d("AndroiX", "mouse not ready. mouse: " + mMousePtr); return false; }
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                Log.d("AndroiX", "onTouchEvent: ACTION_DOWN x: " + event.getX() + " y: " + event.getY());
                AndroiXService.lib.touchDown(mMousePtr, (int)(event.getX()), (int)(event.getY()));
                return true;
            case MotionEvent.ACTION_MOVE:
                Log.d("AndroiX", "onTouchEvent: ACTION_MOVE x: " + event.getX() + " y: " + event.getY());
                AndroiXService.lib.touchMove(mMousePtr, (int)(event.getX()), (int)(event.getY()));
                return true;
            case MotionEvent.ACTION_UP:
                Log.d("AndroiX", "onTouchEvent: ACTION_UP x: " + event.getX() + " y: " + event.getY());
                AndroiXService.lib.touchUp(mMousePtr, (int)(event.getX()), (int)(event.getY()));
                return true;
        };
        return false;
    }

    /* OnTrackballEvent */

    @Override
    public boolean onTrackballEvent(MotionEvent event) {
        if (mMousePtr == 0) { Log.d("AndroiX", "trackball not ready. ball: " + mTrackballPtr); return false; }
        switch (event.getAction()) {
            case MotionEvent.ACTION_MOVE:
                Log.d("AndroiX", "onTrackballEvent: x: " + event.getX() + " y: " + event.getY());
                AndroiXService.lib.trackballNormalizedMotion(mTrackballPtr, event.getX(), event.getY());
                return true;
            case MotionEvent.ACTION_DOWN:
                Log.d("AndroiX", "onTrackballEvent: ACTION_DOWN");
                AndroiXService.lib.trackballPress(mTrackballPtr);
                return true;
            case MotionEvent.ACTION_UP:
                Log.d("AndroiX", "onTrackballEvent: ACTION_UP");
                AndroiXService.lib.trackballRelease(mTrackballPtr);
                return true;
        };
        return false;
    }    

}
