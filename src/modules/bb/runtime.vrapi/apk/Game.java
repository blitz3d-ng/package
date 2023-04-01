package blitz3d.runtimevrapi;

public class Game extends android.app.NativeActivity {
  static {
    System.loadLibrary("vrapi");
    System.loadLibrary("main");
  }
}
