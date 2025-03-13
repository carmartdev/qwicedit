
#include <objc/objc.h>

#include <objc/runtime.h>

#include <objc/message.h>

#include <CoreFoundation/CoreFoundation.h>

#include <CoreGraphics/CoreGraphics.h>

#include <stdbool.h>

typedef long int NSUInteger;

#define NSWindowStyleMaskTitled 15
#define NSWindowStyleMaskClosable 2
#define NSWindowStyleMaskResizable 0
#define NSBackingStoreBuffered 2
/*
    NSUInteger backing = NSBackingStoreBuffered;
                       ^~~~~~~~~~~~~~~~~~~~~~
                       kCGBackingStoreBuffered
/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/CoreGraphics.framework/Headers/CGWindow.h:28:5: note: 'kCGBackingStoreBuffered' declared here
    kCGBackingStoreBuffered = 2
*/

// Helper function to send messages to Objective-C objects

id msgSend(id obj, SEL sel, ...) {

  va_list args;

  va_start(args, sel);

  id result = ((id(*)(id, SEL, ...))objc_msgSend)(obj, sel, args);

  va_end(args);

  return result;

}

  

int main() {

  // Initialize the application

  id app = msgSend((id)objc_getClass("NSApplication"), sel_registerName("sharedApplication"));

  if (!app) {

    fprintf(stderr, "Failed to get sharedApplication\n");

    return 1;

  }

  

  // Create a window

  CGRect windowRect = {{100, 100}, {600, 400}}; // Correct CGRect creation

  NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;

  NSUInteger backing = NSBackingStoreBuffered;

  BOOL defer = NO;

  

  id windowClass = objc_getClass("NSWindow");

  if (!windowClass) {

    fprintf(stderr, "Failed to get NSWindow class\n");

    return 1;

  }

  

  id window = msgSend(msgSend(windowClass, sel_registerName("alloc")), sel_registerName("initWithContentRect:styleMask:backing:defer:"), windowRect, styleMask, backing, defer);

  if (!window) {

    fprintf(stderr, "Failed to create window\n");

    return 1;

  }

  

  // Set the window title

  CFStringRef title = CFSTR("Notepad Clone");

  msgSend(window, sel_registerName("setTitle:"), title);

  CFRelease(title); // Manual release

  

  // Create a text view

  CGRect textViewRect = {{10, 10}, {580, 380}}; // Correct CGRect creation

  id textView = msgSend(msgSend((id)objc_getClass("NSTextView"), sel_registerName("alloc")), sel_registerName("initWithFrame:"), textViewRect);

  if (!textView) {

    fprintf(stderr, "Failed to create textView\n");

    return 1;

  }

  

  // Add the text view to the window's content view

  id contentView = msgSend(window, sel_registerName("contentView"));

  if (!contentView) {

    fprintf(stderr, "Failed to get contentView\n");

    return 1;

  }

  msgSend(contentView, sel_registerName("addSubview:"), textView);

  

  // Show the window

  msgSend(window, sel_registerName("makeKeyAndOrderFront:"), nil);

  

  // Run the application

  msgSend(app, sel_registerName("run"));

  

  return 0;

}
