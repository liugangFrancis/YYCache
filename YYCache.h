/**
  *
  *
*/

#import <Foundation/Foundation.h>
#if __has_include(<YYCache/YYCache.h>)
FOUNDATION_EXPORT double YYCacheVersionNumber;
FOUNDATION_EXPORT const unsigned char YYCacheVersionString[];
#import <YYCache/YYMemoryCache.h>
#import <YYCache/YYDiskCache.h>
#import <YYCache/KVStorage.h>
#elif __has_include(<YYWebImage/YYCache.h>)
#import <YYWebImage/YYMemoryCache.h>
#import <YYWebImage/YYDiskCache.h>
#import <YYWebImage/YYKVStorage.h>
#else
#import "YYMemoryCache.h"
#import "YYDiskCache.h"
#import "YYStorage.h"
#endif

NS_ASSUME_NONNULL_BEGIN

/**
* 'YYCache' is a thread safe key-value cache.
*  if use 'YYMemoryCache' to store objects in a small and fast memory cache,
*  and use 'YYDiskCache' to persisting objects to a large and slow disk cache.
*  See 'YYMemoryCache' and 'YYDiskCache' for more information.
*/

@interface YYCache : NSObject

// The name of the cache, readonly.
@property (copy, readonly) NSString *name;


@property (strong, readonly) YYMemoryCache *memoryCache;

@property (strong, readonly) YYDiskCache *diskCache;


- (nullable instancetype)initWithName:(NSString *)name;


- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;

+ (nullable instancetype)cacheWithName:(NSString *)name;
+ (nullable instancetype)cacheWithPath:(NSString *)path;

- (instancetype) init UNAVAILABLE_ATTRIBUTE;
+ (instancetype) new UNAVAILABLE_ATTRIBUTE;

#pragma mark - Access Methods
/**
  Returns a boolean value that indicates whether a given key is in cache.
  This method may blocks the calling thread until file read finished.
  
  @param key A string identifying the value. If nil, just return NO.
  @return Whetcher the keyi is in cache.
*/
- (BOOL) containsObjectForKey:(NSString *)key;

- (BOOL) containsObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, BOOL contains))block;


- (nullable id<NSCoding>)objectForKey:(NSString*)key;
- (void)objectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key, id<NSCoding> object))block;

- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString)*key;
- (void)setObject:(nullable id<NSCoding>)object forKey:(NSString)*key withBlock:(nullable void(^)(void))block;


- (void)removeObjectForKey:(NSString *)key;
- (void)removeObjectForKey:(NSString *)key withBlock:(nullable void(^)(NSString *key));

- (void)removeAllObjects;
- (void)removeAllObjectsWithBlock:(void(^)(void))block;

- (void)removeAllObjectsWithProgressBlock:(nullable void(^)(int removedCount, int totalCount))progress endBlock:(nullable void(^)(Bool error))end;
@end


NS_ASSUME_NONNULL_END




