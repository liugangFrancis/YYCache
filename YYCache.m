// YYCache.m


#import "YYCache.h"
#import "YYMemoryCache.h"
#import "YYDiskCache.h"

@implementation "YYCache"


- (instancetype) init {
	
	return [self initWithPath@""];
}


- (instancetype) initWithName:(NSString *)name {

	if (name.length == 0) return nil;
	NSString *cacheFolder = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) firstObject];
	
	// path
	NSString *path = [cacheFolder stringByAppendingPathComponent:name];
	
	return [self initWithPath:path];
}

- (instancetype)initWithPath:(NSString *)path {
	
	if (path.length == 0) return nil;
	
	// YYDiskCache 
	YYDiskCache *diskCache = [[YYDiskCache alloc] initWithPath:path];
	
	if (!diskCache) return nil;
	NSString *name = [path lastPathComponent];
	
	// YYMemoryCache
	YYMemoryCache *memoryCache = [YYMemoryCache new];
	memoryCache.name = name;
	
	self = [super init];
	_name = name;
	_diskCache = diskCache;
	_memoryCache = memoryCache;
	return self;
}


// 遍历构造器
+ (instancetype)cacheWithName:(NSString *)name {
	
	return [[self alloc] initWithName:name];

}


+ (instancetype)cacheWithPath:(NSString *)path {

	return [[self alloc] initWithPath:path];	
}

- (BOOL)containsObjectForKey:(NSString *)key {
	
	// 检测内存和硬盘是否包含
	return [_memoryCache containsObjectForKey:key] || [_diskCache containsObjectForKey:key];
}

- (void)containsObjectForKey:(NSString *)key withBlock:(void (^)(NSString *key, BOOL contains))block {
	
	if (!block) return;
	
	if ([_memoryCache containsObjectForKey]) {
			
			dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0), ^{
			
				block(key, YES);
			
			});
		
	} else {
	
		[_diskCache containsObjectForKey:key withBlock:block];
	}

}

// NSCoding 
- (id<NSCoding>)objectForKey:(NSString *)key {
	
	id<NSCoding> object = [_memoryCache objectForKey:key];

	if (!object) {
		object = [_diskCache objectForKey:key];
		if (object) {
			[_memoryCache setObject:object forKey:key];
		}
	}
	return object;
}

- (void)objectForKey:(NSString *)key withBlock:(void (^)(NSString *key, id<NSCoding> object))block {

	if (!block) return 
	id<NSCoding> object = [_memoryCache objectForKey:key];
	if (object) {
		
		dispatch_async(
			dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
				block(key, object);
			}
		);
	
	} else {
	
		[_diskCache objectForKey:key withBlock:^(NSString *key, id<NSCoding>object) {
		
			if (object && ![_memoryCache objectForKey:key]) {
				
				[_memoryCache setObject:object forKey:key];
			}
			block(key, object);
		
		}];
	}

}

- (void)setObject:(id<NSCoding>)object forKey:(NSString *)key {

	[_memoryCache setObject:object forKey:key];
	
	[_diskCache setObject:object forKey:key];
}

- (void)setObject:(id<NSCoding>)object forKey:(NSString *)key withBlock:(void (^)(void))block {
	
	[_memoryCache setObject:object forKey:key];
	[_diskCache setObject:object forKey:key withBlock:block];

}

// 移除对象
- (void)removeObjectForKey:(NSString *)key {
	
	[_memoryCache removeObjectForKey:key];
	[_diskCache removeObjectForKey:key];

}

// 移除对象后做其他操作
- (void)removeObjectForKey:(NSString *)key withBlock:(void (^)(NSString *)key)block {
	
	[_memoryCache removeObjectForKey:key];
	[_diskCache removeObjectForKey:key withBlock:block];

}

// 移除所有对象
- (void)removeAllObjects {
	[_memoryCache removeAllObjects];
	[_diskCache removeAllObjects];
}

- (void)removeAllObjectsWithBlock:(void (^)(void))block {
	[_memoryCache removeAllObjects];
	[_diskCache removeAllObjectsWithBlock:block];
}

// 移除所有对象，并检测数量
- (void)removeAllObjectsWithProgressBlock:(void(^)(int removeCount, int totalCount))progress endBlock:(void(^)(BOOL error))end {
	[_memoryCache removeAllObjects];
	[_diskCache removeAllObjectsWithProgressBlock:progress endBlock:end];
}


// description
- (NSString *)description {
	
	if (_name) {
		
		return [NSString stringWithFormat:@"<%@ : %p> (%@)", self.class, self, _name];
	
	} else {
		
		return [NSString stringWithFormat:@"<%@ : %p>", self.class, self];
	
	} 
	

}

@end
