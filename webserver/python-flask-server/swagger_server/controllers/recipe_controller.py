import connexion
import six

from swagger_server.models.recipe import Recipe  # noqa: E501
from swagger_server.models.recipelist import Recipelist  # noqa: E501
from swagger_server import util

aRecipelist = []
allRecipes = {}

def add_recipe(recipe):  # noqa: E501
    """Add a recipe

    Use this to add a recipe to the list of available recipes on the server # noqa: E501

    :param recipe: Recipe to add to the server
    :type recipe: dict | bytes

    :rtype: None
    """
    if connexion.request.is_json:
        recipe = Recipe.from_dict(connexion.request.get_json())  # noqa: E501
    if not recipe.id in allRecipes:
        allRecipes.update({recipe.id: recipe})
        aRecipelist.append({'id': recipe.id, 'title': recipe.title})
    else:
        return 'Recipe exists already', 405
    return 'Successful operation', 200


def change_recipe(recipeId, recipe):  # noqa: E501
    """Change an existing recipe

    Here we change a recipe that already exists on the server. # noqa: E501

    :param recipeId: ID of recipe to change
    :type recipeId: 
    :param recipe: The changed recipe
    :type recipe: dict | bytes

    :rtype: Recipe
    """
    if not recipeId in allRecipes:
        return 'Recipe not found', 404
    if connexion.request.is_json:
        recipe = Recipe.from_dict(connexion.request.get_json())  # noqa: E501
        allRecipes[recipeId] = recipe
    return allRecipes[recipeId], 200


def delete_recipe(recipeId):  # noqa: E501
    """Deletes a recipe

    Here a recipe is deleted from the server # noqa: E501

    :param recipeId: Id of recipe to delete
    :type recipeId: 

    :rtype: Recipelist
    """
    if not recipeId in allRecipes:
        return 'Recipe not found', 404
    del aRecipelist[aRecipelist.index({'id': recipeId, 'title': allRecipes[recipeId].title})]
    del allRecipes[recipeId]
    return aRecipelist, 200


def get_recipe(recipeId):  # noqa: E501
    """Retrieve a recipe

    Retrieve detailed information about a single recipe. # noqa: E501

    :param recipeId: ID of recipe to return
    :type recipeId: 

    :rtype: Recipe
    """
    if not recipeId in allRecipes:
        return 'Recipe not found', 404
    return allRecipes[recipeId], 200


def get_recipe_image(recipeId):  # noqa: E501
    """Retrieve the image of a recipe

    Retrieve the image of a single recipe. # noqa: E501

    :param recipeId: ID of recipe to return the image of
    :type recipeId: 

    :rtype: file
    """
    return 'do some magic!'


def get_recipe_list():  # noqa: E501
    """Get a list of available recipes

    Here we return a list of all available recipes on the server. The list itself only contains ID and name of the recipe. Use the ID to get all details about the recipe. # noqa: E501


    :rtype: Recipelist
    """
    return aRecipelist, 200


def set_recipe_image(recipeId, file=None):  # noqa: E501
    """Upload the image of a recipe

    Upload the image of a recipe on the server. # noqa: E501

    :param recipeId: ID of recipe to change
    :type recipeId: 
    :param file: Image of recipe to upload
    :type file: werkzeug.datastructures.FileStorage

    :rtype: None
    """
    return 'do some magic!'
