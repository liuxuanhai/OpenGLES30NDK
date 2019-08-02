package cn.root4.opengles30ndk;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class NativeRenderer implements GLSurfaceView.Renderer {

    private static String LOG_TAG = "opengles30ndk_log";

    static {
        System.loadLibrary("native-renderer");
    }

    private native void surfaceCreated(int bgColor);

    private native void surfaceChanged(int width, int height);

    private native void drawFrame();

    private int bgColor;

    public NativeRenderer(int bgColor) {
        this.bgColor = bgColor;
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        Log.i(LOG_TAG, "onSurfaceCreated thread id : " + Thread.currentThread().getId());
        surfaceCreated(bgColor);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        Log.i(LOG_TAG, "onSurfaceChanged thread id : " + Thread.currentThread().getId());
        surfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        Log.i(LOG_TAG, "onDrawFrame thread id : " + Thread.currentThread().getId());
        drawFrame();
    }
}
