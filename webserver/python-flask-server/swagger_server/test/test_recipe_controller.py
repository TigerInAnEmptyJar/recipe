# coding: utf-8

from __future__ import absolute_import

from flask import json
from six import BytesIO

from swagger_server.models.recipe import Recipe  # noqa: E501
from swagger_server.models.recipelist import Recipelist  # noqa: E501
from swagger_server.test import BaseTestCase


class TestRecipeController(BaseTestCase):
    """RecipeController integration test stubs"""

    def test_add_recipe(self):
        """Test case for add_recipe

        Add a recipe
        """
        recipe = Recipe()
        response = self.client.open(
            '/v1/recipe',
            method='POST',
            data=json.dumps(recipe),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_change_recipe(self):
        """Test case for change_recipe

        Change an existing recipe
        """
        recipe = Recipe()
        response = self.client.open(
            '/v1/recipe/{recipeId}'.format(recipeId='recipeId_example'),
            method='PUT',
            data=json.dumps(recipe),
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_delete_recipe(self):
        """Test case for delete_recipe

        Deletes a recipe
        """
        response = self.client.open(
            '/v1/recipe/{recipeId}'.format(recipeId='recipeId_example'),
            method='DELETE')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_recipe(self):
        """Test case for get_recipe

        Retrieve a recipe
        """
        response = self.client.open(
            '/v1/recipe/{recipeId}'.format(recipeId='recipeId_example'),
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_recipe_image(self):
        """Test case for get_recipe_image

        Retrieve the image of a recipe
        """
        response = self.client.open(
            '/v1/recipe/image/{recipeId}'.format(recipeId='recipeId_example'),
            method='GET')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_get_recipe_list(self):
        """Test case for get_recipe_list

        Get a list of available recipes
        """
        response = self.client.open(
            '/v1/recipe',
            method='GET',
            content_type='application/json')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))

    def test_set_recipe_image(self):
        """Test case for set_recipe_image

        Upload the image of a recipe
        """
        data = dict(file=(BytesIO(b'some file data'), 'file.txt'))
        response = self.client.open(
            '/v1/recipe/image/{recipeId}'.format(recipeId='recipeId_example'),
            method='PUT',
            data=data,
            content_type='multipart/form-data')
        self.assert200(response,
                       'Response body is : ' + response.data.decode('utf-8'))


if __name__ == '__main__':
    import unittest
    unittest.main()
