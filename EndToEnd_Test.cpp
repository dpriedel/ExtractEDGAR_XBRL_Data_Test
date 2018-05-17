// =====================================================================================
//
//       Filename:  EndToEndTest.cpp
//
//    Description:  Driver program for end-to-end tests
//
//        Version:  1.0
//        Created:  01/03/2014 11:13:53 AM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  David P. Riedel (dpr), driedel@cox.net
//        License:  GNU General Public License v3
//        Company:
//
// =====================================================================================

	/* This file is part of CollectEDGARData. */

	/* CollectEDGARData is free software: you can redistribute it and/or modify */
	/* it under the terms of the GNU General Public License as published by */
	/* the Free Software Foundation, either version 3 of the License, or */
	/* (at your option) any later version. */

	/* CollectEDGARData is distributed in the hope that it will be useful, */
	/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
	/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
	/* GNU General Public License for more details. */

	/* You should have received a copy of the GNU General Public License */
	/* along with CollectEDGARData.  If not, see <http://www.gnu.org/licenses/>. */


// =====================================================================================
//        Class:
//  Description:
// =====================================================================================


#include <experimental/filesystem>
#include <pqxx/pqxx>

#include <gmock/gmock.h>

#include "ExtractEDGAR_XBRLApp.h"

const fs::path FILE_WITH_XML_10Q{"/vol_DA/EDGAR/Archives/edgar/data/1460602/0001062993-13-005017.txt"};
const fs::path FILE_WITH_XML_10K{"/vol_DA/EDGAR/Archives/edgar/data/google-10k.txt"};
const fs::path FILE_WITHOUT_XML{"/vol_DA/EDGAR/Archives/edgar/data/841360/0001086380-13-000030.txt"};
const fs::path EDGAR_DIRECTORY{"/vol_DA/EDGAR/Archives/edgar/data"};
const fs::path FILE_NO_NAMESPACE_10Q{"/vol_DA/EDGAR/Archives/edgar/data/68270/0000068270-13-000059.txt"};

int G_ARGC = 0;
char** G_ARGV = nullptr;

namespace fs = std::experimental::filesystem;

using namespace testing;


class SingleFileEndToEnd : public Test
{
	public:

        void SetUp() override
        {
		    pqxx::connection c{"dbname=edgar_extracts user=edgar_pg"};
		    pqxx::work trxn{c};

		    // make sure the DB is empty before we start

		    trxn.exec("DELETE FROM xbrl_extracts.edgar_filing_id");
		    trxn.commit();
			c.disconnect();
        }

		int CountRows()
		{
		    pqxx::connection c{"dbname=edgar_extracts user=edgar_pg"};
		    pqxx::work trxn{c};

		    // make sure the DB is empty before we start

		    auto row = trxn.exec1("SELECT count(*) FROM xbrl_extracts.edgar_filing_data");
		    trxn.commit();
			c.disconnect();
			return row[0].as<int>();
		}
};


TEST_F(SingleFileEndToEnd, VerifyCanLoadDataToDBForFileWithXML_10Q)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"-f", FILE_WITH_XML_10Q.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountRows(), 194);
}

TEST_F(SingleFileEndToEnd, VerifyCanLoadDataToDBForFileWithXML_NoNamespace_10Q)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"-f", FILE_NO_NAMESPACE_10Q.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountRows(), 723);
}

TEST_F(SingleFileEndToEnd, VerifyCanLoadDataToDBForFileWithXML_10K)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
		"--begin-date", "2013-Oct-14",
		"--end-date", "2015-12-31",
        "--log-level", "debug",
		"--form", "10-K",
		"-f", FILE_WITH_XML_10K.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountRows(), 1984);
}

class ProcessFolderEndtoEnd : public Test
{
	public:

        void SetUp() override
        {
		    pqxx::connection c{"dbname=edgar_extracts user=edgar_pg"};
		    pqxx::work trxn{c};

		    // make sure the DB is empty before we start

		    trxn.exec("DELETE FROM xbrl_extracts.edgar_filing_id");
		    trxn.commit();
			c.disconnect();
        }

		int CountRows()
		{
		    pqxx::connection c{"dbname=edgar_extracts user=edgar_pg"};
		    pqxx::work trxn{c};

		    // make sure the DB is empty before we start

		    auto row = trxn.exec1("SELECT count(*) FROM xbrl_extracts.edgar_filing_data");
		    trxn.commit();
			c.disconnect();
			return row[0].as<int>();
		}

		int CountFilings()
		{
		    pqxx::connection c{"dbname=edgar_extracts user=edgar_pg"};
		    pqxx::work trxn{c};

		    // make sure the DB is empty before we start

		    auto row = trxn.exec1("SELECT count(*) FROM xbrl_extracts.edgar_filing_id");
		    trxn.commit();
			c.disconnect();
			return row[0].as<int>();
		}
};

TEST_F(ProcessFolderEndtoEnd, WorkWithFileList1)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-K"
    };

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 0);
}

TEST_F(ProcessFolderEndtoEnd, WorkWithFileList2)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-K",
		"--form-dir", EDGAR_DIRECTORY.string()
    };

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 1);
}

TEST_F(ProcessFolderEndtoEnd, WorkWithFileList3_10Q)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"--list", "./test_directory_list.txt"
    };

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 155);
}

TEST_F(ProcessFolderEndtoEnd, WorkWithFileList3WithLimit_10Q)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"--max", "17",
		"--list", "./test_directory_list.txt"
    };

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 17);
}

TEST_F(ProcessFolderEndtoEnd, WorkWithFileList3_10K)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-K",
		"--list", "./test_directory_list.txt"
    };

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 1);
}

TEST_F(ProcessFolderEndtoEnd, VerifyCanApplyFilters)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-K",
		"--form-dir", EDGAR_DIRECTORY.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 1);
}

TEST_F(ProcessFolderEndtoEnd, VerifyCanApplyFilters2)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
		"--begin-date", "2013-Mar-1",
		"--end-date", "2013-3-31",
        "--log-level", "debug",
		"--form", "10-Q",
		"--form-dir", EDGAR_DIRECTORY.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	ASSERT_EQ(CountFilings(), 5);
}

TEST_F(ProcessFolderEndtoEnd, LoadLotsOfFiles)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"--form-dir", EDGAR_DIRECTORY.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	// NOTE: there are 157 files which meet the scan criteria BUT 2 of them are duplicated.
	ASSERT_EQ(CountFilings(), 155);
}

TEST_F(ProcessFolderEndtoEnd, LoadLotsOfFilesWithLimit)
{
	//	NOTE: the program name 'the_program' in the command line below is ignored in the
	//	the test program.

	std::vector<std::string> tokens{"the_program",
        "--log-level", "debug",
		"--form", "10-Q",
		"--max", "14",
		"--form-dir", EDGAR_DIRECTORY.string()
	};

    ExtractEDGAR_XBRLApp myApp;
	try
	{
        myApp.init(tokens);

		decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
		myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");

        myApp.run();
	}

    // catch any problems trying to setup application

	catch (const std::exception& theProblem)
	{
		// poco_fatal(myApp->logger(), theProblem.what());

		myApp.logger().error(std::string("Something fundamental went wrong: ") + theProblem.what());
		throw;	//	so test framework will get it too.
	}
	catch (...)
	{		// handle exception: unspecified
		myApp.logger().error("Something totally unexpected happened.");
		throw;
	}
	// NOTE: there are 157 files which meet the scan criteria BUT 2 of them are duplicated.
	ASSERT_EQ(CountFilings(), 14);
}

// TEST(DailyEndToEndTest, VerifyDownloadsOfExistingFormFilesWhenReplaceIsSpecifed)
// {
// 	if (fs::exists("/tmp/index2"))
// 		fs::remove_all("/tmp/index2");
// 	if (fs::exists("/tmp/forms2"))
// 		fs::remove_all("/tmp/forms2");
// 	fs::create_directory("/tmp/forms2");
//
// 	//	NOTE: the program name 'the_program' in the command line below is ignored in the
// 	//	the test program.
//
// 	std::vector<std::string> tokens{"the_program",
// 		"--index-dir", "/tmp/index2",
// 		"--form-dir", "/tmp/forms2",
//         "--host", "https://localhost:8443",
// 		"--begin-date", "2013-Oct-14",
// 		"--end-date", "2013-Oct-17",
// 		"--replace-form-files"
// 	};
//
// 	ExtractEDGAR_XBRLApp myApp;
//     myApp.init(tokens);
//
// 	decltype(auto) test_info = UnitTest::GetInstance()->current_test_info();
// 	myApp.logger().information(std::string("\n\nTest: ") + test_info->name() + " test case: " + test_info->test_case_name() + "\n\n");
//
//     myApp.run();
// 	// decltype(auto) x1 = CollectLastModifiedTimesForFilesInDirectoryTree("/tmp/forms2");
//
// 	std::this_thread::sleep_for(std::chrono::seconds{1});
//
// 	myApp.run();
// 	// decltype(auto) x2 = CollectLastModifiedTimesForFilesInDirectoryTree("/tmp/forms2");
//
// 	// ASSERT_THAT(x1 == x2, Eq(false));
// 	ASSERT_TRUE(0);
// }


int main(int argc, char** argv) {
	G_ARGC = argc;
	G_ARGV = argv;
	testing::InitGoogleMock(&argc, argv);
   return RUN_ALL_TESTS();
}
