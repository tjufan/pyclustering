/**
*
* Copyright (C) 2014-2017    Andrei Novikov (pyclustering@yandex.ru)
*
* GNU_PUBLIC_LICENSE
*   pyclustering is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   pyclustering is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


#include "gtest/gtest.h"

#include "samples.hpp"

#include "container/kdtree.hpp"

#include <algorithm>


using namespace container;


class utest_kdtree : public ::testing::Test {
protected:
    virtual void SetUp() { }

    virtual void TearDown() { }

protected:
    virtual void InitTestObject(const dataset & points) {
        tree = kdtree();
        for (auto & point : points) {
            tree.insert(point);
        }
    }

    virtual void TemplateTestInsertSearchRemove(const dataset & p_data) {
        tree = kdtree();
        for (std::size_t index = 0; index < p_data.size(); index++) {
            ASSERT_EQ(tree.get_size(), index);

            tree.insert(p_data[index]);
            ASSERT_EQ(tree.get_size(), index + 1);

            kdtree_searcher searcher(p_data[index], tree.get_root(), 10.0);
            kdnode::ptr nearest_node = searcher.find_nearest_node();

            ASSERT_EQ(nearest_node->get_data(), p_data[index]);


            std::vector<double> nearest_distances;
            std::vector<kdnode::ptr> nearest_nodes;
            searcher.find_nearest_nodes(nearest_distances, nearest_nodes);

            ASSERT_EQ(tree.get_size(), nearest_distances.size());
            ASSERT_EQ(tree.get_size(), nearest_nodes.size());

            for (auto & found_kdnode : nearest_nodes) {
                ASSERT_NE(nullptr, found_kdnode);
            }
        }

        for (std::size_t index = 0; index < p_data.size(); index++) {
            ASSERT_EQ(tree.get_size(), p_data.size() - index);

            tree.remove(p_data[index]);
            ASSERT_EQ(tree.get_size(), p_data.size() - index - 1);
        }

        ASSERT_EQ(tree.get_size(), 0);
        ASSERT_EQ(tree.get_root(), nullptr);
    }

protected:
    kdtree tree;
};


TEST_F(utest_kdtree, creation_without_payload) {
    std::vector< std::vector<double> > test_sample_point_vector = { {4, 3}, {3, 4}, {5, 8}, {3, 3}, {3, 9}, {6, 4}, {5, 9} };

    InitTestObject(test_sample_point_vector);

    for (auto & point : test_sample_point_vector) {
        kdnode::ptr node = tree.find_node(point);

        ASSERT_NE(nullptr, node);
        ASSERT_EQ(nullptr, node->get_payload());
        ASSERT_EQ(point, node->get_data());
    }
}


TEST_F(utest_kdtree, parent_node) {
    std::vector< std::vector<double> > test_sample_point_vector = { {4, 3}, {3, 4}, {5, 8}, {3, 3}, {3, 9}, {6, 4}, {5, 9} };

    InitTestObject(test_sample_point_vector);

    kdnode::ptr node = tree.find_node(test_sample_point_vector[0]);
    ASSERT_EQ(nullptr, node->get_parent());

    node = tree.find_node(test_sample_point_vector[1]);
    ASSERT_EQ(test_sample_point_vector[0], node->get_parent()->get_data());

    node = tree.find_node(test_sample_point_vector[2]);
    ASSERT_EQ(test_sample_point_vector[0], node->get_parent()->get_data());

    node = tree.find_node(test_sample_point_vector[5]);
    ASSERT_EQ(test_sample_point_vector[2], node->get_parent()->get_data());

    node = tree.find_node(test_sample_point_vector[3]);
    ASSERT_EQ(test_sample_point_vector[1], node->get_parent()->get_data());

    node = tree.find_node(test_sample_point_vector[6]);
    ASSERT_EQ(test_sample_point_vector[2], node->get_parent()->get_data());

    node = tree.find_node(test_sample_point_vector[4]);
    ASSERT_EQ(test_sample_point_vector[1], node->get_parent()->get_data());
}


TEST_F(utest_kdtree, insert_remove_node) {
    std::vector< char > test_sample_payload = { 'q', 'w', 'e', 'r', 't', 'y', 'u' };
    std::vector< std::vector<double> > test_sample_point_vector = { {4, 3}, {3, 4}, {5, 8}, {3, 3}, {3, 9}, {6, 4}, {5, 9} };

    for (std::size_t index = 0; index < test_sample_point_vector.size(); index++) {
        std::vector<double> & point = test_sample_point_vector[index];
        char * payload = (char *) &(test_sample_payload[index]);

        tree.insert(point, payload);

        kdnode::ptr node = tree.find_node(point);
        ASSERT_NE(nullptr, node.get());
        ASSERT_EQ(point, node->get_data());
        ASSERT_EQ(payload, node->get_payload());
    }


    for (std::size_t index = 0; index < test_sample_point_vector.size(); index++) {
        std::vector<double> & point = test_sample_point_vector[index];
        tree.remove(point);

        kdnode::ptr node = tree.find_node(test_sample_point_vector[index]);
        ASSERT_EQ(nullptr, node.get());

        for (std::size_t next_index = index + 1; next_index < test_sample_point_vector.size(); next_index++) {
            node = tree.find_node(test_sample_point_vector[next_index]);
            std::vector<double> point = test_sample_point_vector[next_index];

            ASSERT_EQ(point, node->get_data());
            ASSERT_EQ(&test_sample_payload[next_index], node->get_payload());
        }
    }
}


TEST_F(utest_kdtree, find_without_insertion) {
    std::vector< std::vector<double> > test_sample_point_vector = { {4, 3}, {3, 4}, {5, 8}, {3, 3}, {3, 9}, {6, 4}, {5, 9} };

    for (unsigned int index = 0; index < test_sample_point_vector.size(); index++) {
        std::vector<double> & point = test_sample_point_vector[index];

        kdnode::ptr node = tree.find_node(point);
        ASSERT_EQ(nullptr, node.get());
    }
}


TEST_F(utest_kdtree, remove_long_branch) {
    std::vector< std::vector<double> > test_sample_point_vector = { {5, 5}, {6, 5}, {6, 6}, {7, 6}, {7, 7} };

    InitTestObject(test_sample_point_vector);

    for (auto & point : test_sample_point_vector) {
        tree.remove(point);

        kdnode::ptr node = tree.find_node(point);
        ASSERT_EQ(nullptr, node);
    }
}


TEST_F(utest_kdtree, insert_search_remove_simple_01) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_01);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_02) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_02);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_03) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_03);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_04) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_04);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_05) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_05);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_06) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_06);
    TemplateTestInsertSearchRemove(*sample);
}


TEST_F(utest_kdtree, insert_search_remove_simple_07) {
    auto sample = simple_sample_factory::create_sample(SAMPLE_SIMPLE::SAMPLE_SIMPLE_07);
    TemplateTestInsertSearchRemove(*sample);
}