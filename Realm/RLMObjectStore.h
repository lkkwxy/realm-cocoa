////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

@class RLMRealm, RLMSchema, RLMObjectSchema, RLMObjectBase, RLMResults;

//
// Table modifications
//

// updates a Realm to a given target schema/version
// creates tables as necessary
// optionally runs migration block if schema is out of date
//
// NOTE: the schema passed in will be set on the Realm and may later be mutated. sharing a targetSchema accross
// even the same Realm with different column orderings will cause issues
NSError *RLMUpdateRealmToSchemaVersion(RLMRealm *realm, NSUInteger version, RLMSchema *targetSchema, NSError *(^migrationBlock)());

// sets a realm's schema to a copy of targetSchema
// caches table accessors on each objectSchema
//
// NOTE: the schema passed in will be set on the Realm and may later be mutated. sharing a targetSchema accross
// even the same Realm with different column orderings will cause issues
void RLMRealmSetSchema(RLMRealm *realm, RLMSchema *targetSchema, bool verifyAndAlignColumns);

// create or get cached accessors for the given schema
void RLMRealmCreateAccessors(RLMSchema *schema);

// Clear the cache of created accessor classes
void RLMClearAccessorCache();

//
// Options for object creation
//
typedef NS_OPTIONS(NSUInteger, RLMCreationOptions) {
    // Normal object creation
    RLMCreationOptionsNone = 0,
    // Verify that no existing row has the same value for this property
    RLMCreationOptionsEnforceUnique = 1 << 0,
    // If the property is a link or array property, upsert the linked objects
    // if they have a primary key, and insert them otherwise.
    RLMCreationOptionsUpdateOrCreate = 1 << 1,
    // If a link or array property links to an object persisted in a different
    // realm from the object, copy it into the object's realm rather than throwing
    // an error
    RLMCreationOptionsAllowCopy = 1 << 2,
};


//
// Adding, Removing, Getting Objects
//

// add an object to the given realm
void RLMAddObjectToRealm(RLMObjectBase *object, RLMRealm *realm, RLMCreationOptions options);

// delete an object from its realm
void RLMDeleteObjectFromRealm(RLMObjectBase *object, RLMRealm *realm);

// deletes all objects from a realm
void RLMDeleteAllObjectsFromRealm(RLMRealm *realm);

// get objects of a given class
RLMResults *RLMGetObjects(RLMRealm *realm, NSString *objectClassName, NSPredicate *predicate);

// get an object with the given primary key
id RLMGetObject(RLMRealm *realm, NSString *objectClassName, id key);

// create object from array or dictionary
RLMObjectBase *RLMCreateObjectInRealmWithValue(RLMRealm *realm, NSString *className, id value, RLMCreationOptions options);

RLMObjectBase *_RLMCreateObjectInRealmWithValue(RLMRealm *realm, NSString *className, id value, RLMCreationOptions options, NSMapTable *mapping);


//
// Accessor Creation
//

// Create accessors
RLMObjectBase *RLMCreateObjectAccessor(__unsafe_unretained RLMRealm *const realm,
                                       __unsafe_unretained RLMObjectSchema *const objectSchema,
                                       NSUInteger index);

void RLMInitializeSwiftListAccessor(RLMObjectBase *object);

#ifdef __cplusplus
}
#endif
