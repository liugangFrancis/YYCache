
#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN


@interface YYMemoryCache : NSObject

/**
	YYMemoryCache is a fast in-memory cache that stores key-value pairs.
	
	// in contrast to 与字典对比
	In contrast to NSDictionary, keys are retained and not copied.
	
	The API and performance is similar to 'NSCache', all methods are thread-safe.
	
	YYMemoryCache objects differ from NSCache in a few ways:
	
	// 最近最少使用算法
	1. It use LRU(least-recently-used) to remove objects; NSCache's eviction method is no-deterministic.
	
	// 比NSCache更精确
	2. It can be controlled by cost, count and age;
	NSCache's limits are imprecise
	
	// 它可以在接收到内存警告或者应用进入后台时，自动清除对象
	3. It can be configuared to automatically evict objects when receive memory warning or app enter background.
	
	The time of 'Access Methods' in YYMemoryCache is typically in constant time(O(1))
	
	
*/
#pragma park - Attribute

// The name of the cache. Default is nil
@property (nullalbe, copy) NSString *name;

// The number of objects in the cache (read-only)
@property (readonly) NSUInteger totalCount;


// The total cost of objects in the cache (read-only)
@property (readonly) NSUInteger totalCost;


#pragma mark - Limit

/**
	The maximum number of objects the cache should hold.
	
	@discussion The default value is NSUIntegerMax, which means no limit.
	This is not a strict limit-if the cache goes over the limit, some objects in the cache could be evicted later in background thread.
*/
@property NSUInteger countLimit;


/**
* The maximum total cost that the cache can hold before it starts evicting objects.

  @discussion The default value is NSUIntegerMax, which means no limit.
  This is not a strict limit-if the cache goes over the limit, some objects could be evicted later in background thread.
*/
@property NSUInteger costLimit;

/**
 The maximum expiry time of objects in cache.
 
 @discussion The default value is DBL_MAX, which means no limit.
 This is not a strict limit—if an object goes over the limit, the object could 
 be evicted later in backgound thread.
 */
@property NSTimeInterval ageLimit;


/**
	The auto trim check time interval in seconds. Default is 5.0.
	
	@discussion The cache holds an interval timer to check whether the cache reaches its limits, and if the limit is reached, it begins to evict objects.
*/
@property NSTimeInterval autoTrimInterval;

// The default value is 'YES'. if 'YES', the cache will remove all objects when the app receives a memory warning.

@property BOOL shouldRemoveAllObjectsOnMemoryWarning;


@property BOOL shouldRemoveAllObjectsWhenEnteringBackground;

// A block to be excuted when the app receives a memory warning. The default value is nil

@property (nullable, copy) void(^didReceiveMemoryWarningBlock)(YYMemoryCache *cache);


// A block to be excuted when the app enter background.The default value is nil
@property (nullable, copy) void(^didEnterBackgroundBlock)(YYMemoryCache *cache);


/**
	if yes, the key-value pair will be released on main thread,otherwise on background thread.Default is NO.
	
	@discussion You may set this value to yes if the key-value object contains the instance which should be released in main thread (such as UIView/CALayer)

*/
@property BOOL releaseOnMainThread;


/**
 If `YES`, the key-value pair will be released asynchronously to avoid blocking 
 the access methods, otherwise it will be released in the access method  
 (such as removeObjectForKey:). Default is YES.
 */
@property BOOL releaseAsynchronously;


#pragma mark - Access Methods

- (BOOL)containsObjectForKey:(id)key;

- (nullable id)objectForKey:(id)key;

- (void)setObject:(nullable id)object forKey:(id)key;

- (void)setObject:(nullable id)object forKey:(id)key withCost:(NSUInteger)cost;

- (void)removeObjectForKey:(id)key;

- (void)removeAllObjects;


#pragma mark - Trim

- (void)trimToCount:(NSUInteger)count;

- (void)trimToCost:(NSUInteger)cost;

- (void)trimToAge:(NSTimeInterval)age;

@end

NS_ASSUME_NONNULL_END
