#include "Basic.h"
#include "CoreUObject_Classes.h"

#include "../../../Utilities/Math.h"

bool SDK::FVector::Normalize(float Tolerance)
{
    const float SquareSum = X * X + Y * Y + Z * Z;
    if (SquareSum > Tolerance)
    {
        const float Scale = Math::InvSqrt(SquareSum);
        X *= Scale; Y *= Scale; Z *= Scale;
        return true;
    }
    return false;
}

class SDK::UObject* SDK::FWeakObjectPtr::Get() const
{
    return SDK::UObject::ObjectArray.GetByIndex(ObjectIndex);
}

class SDK::UObject* SDK::FWeakObjectPtr::operator->() const
{
    return SDK::UObject::ObjectArray.GetByIndex(ObjectIndex);
}

bool SDK::FWeakObjectPtr::operator==(const FWeakObjectPtr& Other) const
{
    return ObjectIndex == Other.ObjectIndex;
}
bool SDK::FWeakObjectPtr::operator!=(const FWeakObjectPtr& Other) const
{
    return ObjectIndex != Other.ObjectIndex;
}

bool SDK::FWeakObjectPtr::operator==(const class UObject* Other) const
{
    return ObjectIndex == Other->Index;
}
bool SDK::FWeakObjectPtr::operator!=(const class UObject* Other) const
{
    return ObjectIndex != Other->Index;
}