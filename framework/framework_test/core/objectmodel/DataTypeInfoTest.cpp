
#include <gtest/gtest.h>
#include <sofa/core/objectmodel/Data.h>
#include <sofa/defaulttype/DataTypeInfo.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <iostream>

namespace sofa
{

using namespace defaulttype;

TEST( DataFundamentalTypeInfoTest, checkDataTypeInfoFundamentalIsOk)
{
    Data< int > d_int("integer");
    const AbstractTypeInfo* typeInfo = d_int.getValueTypeInfo();

    EXPECT_EQ( typeInfo->FixedSize(), true );
    EXPECT_EQ( typeInfo->BaseType()->FixedSize(), true );

    EXPECT_EQ( typeInfo->size( d_int.getValueVoidPtr() ) , 1 );
    EXPECT_EQ( typeInfo->size(), 1 );
}

TEST( DataContainerTypeInfoTest, checkDataTypeInfoVectorSizeIsOk )
{
    Data< Vec3Types::VecCoord > d_x("position");
    sofa::helper::WriteAccessor< Data< Vec3Types::VecCoord > > x = d_x;
    x.resize( 10 );
    const AbstractTypeInfo* typeInfo = d_x.getValueTypeInfo();
    
    EXPECT_EQ( typeInfo->BaseType()->FixedSize(), true );

    EXPECT_EQ( typeInfo->size( d_x.getValueVoidPtr() ) / typeInfo->size() , 10 );
}

TEST( DataContainerTypeInfoTest, checkDataTypeInfoVectorOfVectorSizeIsOk )
{
    typedef sofa::helper::vector< sofa::helper::vector< Vec3Types::Coord > > VectorOfVectorOfCoord;
    Data< VectorOfVectorOfCoord > d_x("position");
    sofa::helper::WriteAccessor< Data< VectorOfVectorOfCoord > > x = d_x;
    x.resize( 3 );
    x[0].resize( 1 );
    x[1].resize( 2 );
    x[2].resize( 3 );
    const AbstractTypeInfo* typeInfo = d_x.getValueTypeInfo();

    EXPECT_EQ( typeInfo->BaseType()->FixedSize(), false );

    EXPECT_EQ( typeInfo->size( ), 3 );
}


struct MyType
{
    static std::string myString;
    /// Output stream
    inline friend std::ostream& operator<< ( std::ostream& out, const MyType& /*gp*/  )
    {
        return out;
    }
    /// Input stream
    inline friend std::istream& operator>> ( std::istream& in, MyType& /*gp*/ )
    {
        return in;
    }
};

std::string MyType::myString = "MyType::myString";

struct DataTypeInfoMyType_test : public ::testing::Test
{
    typedef sofa::helper::vector<MyType> VectorMyType;
    typedef sofa::Data<VectorMyType> DataVectorMyType;
    typedef sofa::Data<MyType> DataMyType;
    typedef sofa::Data<sofa::helper::vector<MyType> > DataVectorMyType; 
    typedef sofa::defaulttype::DataTypeInfo<MyType>   DataTypeInfoMyType;

    static std::size_t size;

    DataTypeInfoMyType_test()
    :myTypeInfo(NULL)
    ,myVectorTypeInfo(NULL)
    ,myValueVoidPtr(NULL)
    ,myVectorValueVoidPtr(NULL)
    {

    }

    void SetUp()
    {
        myTypeInfo     = myData.getValueTypeInfo();
        myValueVoidPtr = myData.getValueVoidPtr();

        myVectorData.setValue( VectorMyType( DataTypeInfoMyType_test::size ) );
        myVectorTypeInfo     = myVectorData.getValueTypeInfo();
        myVectorValueVoidPtr = myVectorData.getValueVoidPtr(); 
    }
    
    DataMyType              myData;
    DataVectorMyType        myVectorData;
    const AbstractTypeInfo* myTypeInfo;
    const AbstractTypeInfo* myVectorTypeInfo;
    const void*             myValueVoidPtr;
    const void*             myVectorValueVoidPtr;
};

std::size_t DataTypeInfoMyType_test::size = 7;

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotValidInfo )
{
    ASSERT_EQ(myTypeInfo->ValidInfo(),false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotFixedSize )
{
    ASSERT_EQ( myTypeInfo->FixedSize(), false );
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotZeroConstructor )
{
    ASSERT_EQ(myTypeInfo->ZeroConstructor(), false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotSimpleCopy )
{
    ASSERT_EQ(myTypeInfo->SimpleCopy(), false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotSimpleLayout )
{
    ASSERT_EQ(myTypeInfo->SimpleLayout(), false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotInteger )
{
    ASSERT_EQ( myTypeInfo->Integer(), false );
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotScalar )
{
    ASSERT_EQ( myTypeInfo->Scalar(), false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotText )
{
    ASSERT_EQ( myTypeInfo->Text(), false );
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoIsNotCopyOnWrite )
{
    ASSERT_EQ(myTypeInfo->CopyOnWrite(), false);
}

TEST_F(DataTypeInfoMyType_test, checkAbstractTypeInfoSizeEqualsOne)
{
    ASSERT_EQ( myTypeInfo->size(), std::size_t(1) );
}

TEST_F(DataTypeInfoMyType_test, checkVectorAbstractTypeInfoSizeIsOK)
{
    ASSERT_EQ( myVectorTypeInfo->size(myVectorValueVoidPtr), DataTypeInfoMyType_test::size );
}

TEST_F(DataTypeInfoMyType_test, checkVectorAbstractTypeInfoSizeEqualsDataTypeInfoSize )
{
    ASSERT_EQ( myTypeInfo->size(), DataTypeInfoMyType_test::DataTypeInfoMyType::size() );
}

TEST_F(DataTypeInfoMyType_test, checkVectorAbstractTypeInfoIsNotValid )
{
    ASSERT_EQ( myTypeInfo->ValidInfo(),false );
}

TEST_F(DataTypeInfoMyType_test, checkVectorAbstractTypeInfoBaseTypeIsNotValid )
{
    ASSERT_EQ( myTypeInfo->BaseType()->ValidInfo() ,false );
}

// This test does not work because the default implementation of DataTypeInfo<DataType>::getValueString 
// is an empty method.
//TEST_F(DataTypeInfoMyType_test, checkVectorAbstractTypeInfoGetValueString )
//{
//    for(std::size_t i=0;i<myVectorTypeInfo->size();++i)
//    {
//        //ASSERT_STREQ(myVectorTypeInfo->getTextValue( myVectorValueVoidPtr, i).c_str(), 
//        //             MyType::myString.c_str() );
//
//    }
//
//}

}
