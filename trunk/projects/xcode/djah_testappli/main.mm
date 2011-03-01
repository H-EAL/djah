#import <UIKit/UIKit.h>

#include "system/ios/iosdevice.hpp"

//--------------------------------------------------------------------------------------------------
@implementation DjahMainApp

- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	djah::system::create_device(480,320);
}

@end
//--------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, @"DjahMainApp", @"DjahMainApp");
    [pool release];
    return retVal;
}
