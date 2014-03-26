/*	NSRunLoop.h
	Copyright (c) 1994-2013, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>
#import <CoreFoundation/CFRunLoop.h>

@class NSTimer, NSPort, NSArray;

FOUNDATION_EXPORT NSString * const NSDefaultRunLoopMode;
FOUNDATION_EXPORT NSString * const NSRunLoopCommonModes NS_AVAILABLE(10_5, 2_0);

@interface NSRunLoop : NSObject {
@private
    id          _rl;
    id          _dperf;
    id          _perft;
    id          _info;
    id		_ports;
    void	*_reserved[6];
}
//获得当前线程的run loop
+ (NSRunLoop *)currentRunLoop;
//获得主线程的run loop
+ (NSRunLoop *)mainRunLoop NS_AVAILABLE(10_5, 2_0);

//  当前的运行模式，只有在运行的状态下返回才有效。
- (NSString *)currentMode;
- (CFRunLoopRef)getCFRunLoop;

- (void)addTimer:(NSTimer *)timer forMode:(NSString *)mode;

- (void)addPort:(NSPort *)aPort forMode:(NSString *)mode;
- (void)removePort:(NSPort *)aPort forMode:(NSString *)mode;


- (NSDate *)limitDateForMode:(NSString *)mode;

//似乎和runMode:差不多（测试过是这种结果，但确定是否有其它特殊情况下的不同），没有BOOL返回值。
- (void)acceptInputForMode:(NSString *)mode beforeDate:(NSDate *)limitDate;

@end
// 使用run loop的一个好处就是避免线程轮询的开销，run loop在无事件处理时可以自动进入睡眠状态，降低CPU的能耗。
@interface NSRunLoop (NSRunLoopConveniences)
// 官网文档也提到run和runUntilDate:会以NSDefaultRunLoopMode参数调用runMode:来处理事件。
//  进入处理事件循环，如果没有事件则立刻返回。
//  注意：主线程上调用这个方法会导致无法返回（进入无限循环，虽然不会阻塞主线程），因为主线程一般总是会有事件处理。
- (void)run;

//同run方法，增加超时参数limitDate，避免进入无限循环。使用在UI线程（亦即主线程）上，可以达到暂停的效果。
// 主要是避免死循环
- (void)runUntilDate:(NSDate *)limitDate;

//等待消息处理，好比在PC终端窗口上等待键盘输入。一旦有合适事件（mode相当于定义了事件的类型）被处理了，则立刻返回；
//类同run方法，如果没有事件处理也立刻返回；有否事件处理由返回布尔值判断。同样limitDate为超时参数。
- (BOOL)runMode:(NSString *)mode beforeDate:(NSDate *)limitDate;

#if (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_IPHONE))
- (void)configureAsServer NS_DEPRECATED(10_0, 10_5, 2_0, 2_0);
#endif

@end

/**************** 	Delayed perform	 ******************/

@interface NSObject (NSDelayedPerforming)

- (void)performSelector:(SEL)aSelector withObject:(id)anArgument afterDelay:(NSTimeInterval)delay inModes:(NSArray *)modes;
- (void)performSelector:(SEL)aSelector withObject:(id)anArgument afterDelay:(NSTimeInterval)delay;
+ (void)cancelPreviousPerformRequestsWithTarget:(id)aTarget selector:(SEL)aSelector object:(id)anArgument;
+ (void)cancelPreviousPerformRequestsWithTarget:(id)aTarget;

@end

@interface NSRunLoop (NSOrderedPerform)

- (void)performSelector:(SEL)aSelector target:(id)target argument:(id)arg order:(NSUInteger)order modes:(NSArray *)modes;
- (void)cancelPerformSelector:(SEL)aSelector target:(id)target argument:(id)arg;
- (void)cancelPerformSelectorsWithTarget:(id)target;

@end

