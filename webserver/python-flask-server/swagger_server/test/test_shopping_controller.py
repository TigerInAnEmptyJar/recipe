# coding: utf-8

from __future__ import absolute_import

from flask import json
from six import BytesIO

from swagger_server.models.shopping import Shopping  # noqa: E501
from swagger_server.models.shopping_ingredient import ShoppingIngredient  # noqa: E501
from swagger_server.models.shoppinglist import Shoppinglist  # noqa: E501
from swagger_server.test import BaseTestCase


class TestShoppingController(BaseTestCase):
    """ShoppingController integration test stubs"""

    def test_add_shoppinglist(self):
        """Test case for add_shoppinglist

        Add a shopping list
        """
        shopping_list = Shopping()
        response = self.client.open(
            '/v1/shopping',
            method='PUT',
            data=json.dumps(shopping_list),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_shoppinglist(self):
        """Test case for get_shoppinglist

        Get a shopping list
        """
        response = self.client.open(
            '/v1/shopping/{shoppingId}'.format(shoppingId='shoppingId_example'),
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_shoppinglists(self):
        """Test case for get_shoppinglists

        Get the list of shopping-lists
        """
        response = self.client.open(
            '/v1/shopping',
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_set_available(self):
        """Test case for set_available

        Set the amounts found on the shelves
        """
        ingredient = ShoppingIngredient()
        response = self.client.open(
            '/v1/shopping/{shoppingId}/{listId}/found'.format(shoppingId='shoppingId_example', listId='listId_example'),
            method='PUT',
            data=json.dumps(ingredient),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    import unittest
    unittest.main()
