package cn.root4.opengles30ndk;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class NativeRenderer implements GLSurfaceView.Renderer {

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
        surfaceCreated(bgColor);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        surfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        drawFrame();
    }
}
