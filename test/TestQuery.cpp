#include "tightdb.h"
#include <UnitTest++.h>

TDB_TABLE_2(TupleTableType,
	Int, first,
	String, second)


TEST(TestQueryFindAll1) {
	TupleTableType ttt;

	ttt.Add(1, "a");
	ttt.Add(2, "a");
	ttt.Add(3, "X");
	ttt.Add(4, "a");
	ttt.Add(5, "a");
	ttt.Add(6, "X");
	ttt.Add(7, "X");

	Query q1 = ttt.Query()->second.Equal("a").first.Greater(2).first.NotEqual(4);
	TableView tv1 = q1.FindAll(ttt);
	CHECK_EQUAL(4, tv1.GetRef(0));

	Query q2 = ttt.Query()->second.Equal("X").first.Greater(4);
	TableView tv2 = q2.FindAll(ttt);
	CHECK_EQUAL(5, tv2.GetRef(0));
	CHECK_EQUAL(6, tv2.GetRef(1));
}

TEST(TestQueryFindAll2) {
	TupleTableType ttt;

	ttt.Add(1, "a");
	ttt.Add(2, "a");
	ttt.Add(3, "X");
	ttt.Add(4, "a");
	ttt.Add(5, "a");
	ttt.Add(11, "X");
	ttt.Add(0, "X");

	Query q2 = ttt.Query()->second.NotEqual("a").first.Less(3);
	TableView tv2 = q2.FindAll(ttt);
	CHECK_EQUAL(6, tv2.GetRef(0));
}

TEST(TestQueryFindAllBetween) {
	TupleTableType ttt;

	ttt.Add(1, "a");
	ttt.Add(2, "a");
	ttt.Add(3, "X");
	ttt.Add(4, "a");
	ttt.Add(5, "a");
	ttt.Add(11, "X");
	ttt.Add(3, "X");

	Query q2 = ttt.Query()->first.Between(3, 5);
	TableView tv2 = q2.FindAll(ttt);
	CHECK_EQUAL(2, tv2.GetRef(0));
	CHECK_EQUAL(3, tv2.GetRef(1));
	CHECK_EQUAL(4, tv2.GetRef(2));
	CHECK_EQUAL(6, tv2.GetRef(3));
}


TEST(TestQueryFindAll_Range) {
	TupleTableType ttt;

	ttt.Add(5, "a");
	ttt.Add(5, "a");
	ttt.Add(5, "a"); 

	Query q1 = ttt.Query()->second.Equal("a").first.Greater(2).first.NotEqual(4);
	TableView tv1 = q1.FindAll(ttt, 1, 2);
	CHECK_EQUAL(1, tv1.GetRef(0));
}


TEST(TestQueryFindAll_Or) {
	TupleTableType ttt;

	ttt.Add(1, "a");
	ttt.Add(2, "a");
	ttt.Add(3, "X");
	ttt.Add(3, "X"); 
	ttt.Add(4, "a");
	ttt.Add(5, "a");
	ttt.Add(11, "X");

	// first > 3 && (first == 5 || second == X)
	Query q1 = ttt.Query()->first.Greater(3).LeftParan().first.Equal(5).Or().second.Equal("X").RightParan();
	TableView tv1 = q1.FindAll(ttt);
	CHECK_EQUAL(5, tv1.GetRef(0));
	CHECK_EQUAL(6, tv1.GetRef(1));
}

TEST(TestQueryFindAll_OrNested) {
	TupleTableType ttt;

	ttt.Add(1, "a");
	ttt.Add(2, "a");
	ttt.Add(3, "X");
	ttt.Add(3, "X"); 
	ttt.Add(4, "a");
	ttt.Add(5, "a");
	ttt.Add(11, "X");
	ttt.Add(8, "Y");
	
	// first > 3 && (first == 5 || (second == X || second == Y))
	Query q1 = ttt.Query()->first.Greater(3).LeftParan().first.Equal(5).Or().LeftParan().second.Equal("X").Or().second.Equal("Y").RightParan().RightParan();
	TableView tv1 = q1.FindAll(ttt);
	CHECK_EQUAL(5, tv1.GetRef(0));
	CHECK_EQUAL(6, tv1.GetRef(1));
	CHECK_EQUAL(7, tv1.GetRef(2));
}

