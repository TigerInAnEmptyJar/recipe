import connexion
import six

from swagger_server.models.shopping import Shopping  # noqa: E501
from swagger_server.models.shopping_ingredient import ShoppingIngredient  # noqa: E501
from swagger_server.models.shoppinglist import Shoppinglist  # noqa: E501
from swagger_server import util

allShopping = {}
aShoppinglist = []

def add_shoppinglist(shopping_list):  # noqa: E501
    """Add a shopping list

    Adds a shopping list on the server. Note: this is only possible for an already existing meal-plan. # noqa: E501

    :param shopping_list: The shopping list for one existing meal-plan
    :type shopping_list: dict | bytes

    :rtype: None
    """
    if connexion.request.is_json:
        shopping_list = Shopping.from_dict(connexion.request.get_json())  # noqa: E501
    if not shopping_list.id in allShopping:
        allShopping.update({shopping_list.id: shopping_list})
        aShoppinglist.append({'id': shopping_list.id, 'title': shopping_list.title})
    else:
        return 'Shopping list exists already', 405
    return 'Successful operation', 200


def get_shoppinglist(shoppingId):  # noqa: E501
    """Get a shopping list

    Retrieve a shopping list from the server # noqa: E501

    :param shoppingId: The Id of the shopping list to retrieve
    :type shoppingId: 

    :rtype: Shopping
    """
    if not shoppingId in allShopping:
        return 'Shoppinglist not found', 404
    return allShopping[shoppingId], 200


def get_shoppinglists():  # noqa: E501
    """Get the list of shopping-lists

    Retrieve the list of shopping lists on the server. # noqa: E501


    :rtype: Shoppinglist
    """
    return aShoppinglist, 200


def set_available(shoppingId, listId, ingredient):  # noqa: E501
    """Set the amounts found on the shelves

    For a specified ingredient on the list, this function here sets the amount that is found at home. # noqa: E501

    :param shoppingId: The Id of the shopping list to modify
    :type shoppingId: 
    :param listId: The Id of the item of the shopping list to modify
    :type listId: 
    :param ingredient: This is the ingredient and its amount, which is found at home
    :type ingredient: dict | bytes

    :rtype: None
    """
    if connexion.request.is_json:
        ingredient = ShoppingIngredient.from_dict(connexion.request.get_json())  # noqa: E501
    return 'do some magic!'
