#import <UIKit/UIKit.h>

int iosScreenWidth(){
  return CGRectGetWidth([UIScreen mainScreen].nativeBounds);
}

int iosScreenHeight(){
  return CGRectGetHeight([UIScreen mainScreen].nativeBounds);
}
