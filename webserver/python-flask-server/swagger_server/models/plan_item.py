# coding: utf-8

from __future__ import absolute_import
from datetime import date, datetime  # noqa: F401

from typing import List, Dict  # noqa: F401

from swagger_server.models.base_model_ import Model
from swagger_server import util


class PlanItem(Model):
    """NOTE: This class is auto generated by the swagger code generator program.

    Do not edit the class manually.
    """

    def __init__(self, shopping_before: bool=None, name: str=None, recipes: List[object]=None):  # noqa: E501
        """PlanItem - a model defined in Swagger

        :param shopping_before: The shopping_before of this PlanItem.  # noqa: E501
        :type shopping_before: bool
        :param name: The name of this PlanItem.  # noqa: E501
        :type name: str
        :param recipes: The recipes of this PlanItem.  # noqa: E501
        :type recipes: List[object]
        """
        self.swagger_types = {
            'shopping_before': bool,
            'name': str,
            'recipes': List[object]
        }

        self.attribute_map = {
            'shopping_before': 'shoppingBefore',
            'name': 'name',
            'recipes': 'recipes'
        }

        self._shopping_before = shopping_before
        self._name = name
        self._recipes = recipes

    @classmethod
    def from_dict(cls, dikt) -> 'PlanItem':
        """Returns the dict as a model

        :param dikt: A dict.
        :type: dict
        :return: The PlanItem of this PlanItem.  # noqa: E501
        :rtype: PlanItem
        """
        return util.deserialize_model(dikt, cls)

    @property
    def shopping_before(self) -> bool:
        """Gets the shopping_before of this PlanItem.


        :return: The shopping_before of this PlanItem.
        :rtype: bool
        """
        return self._shopping_before

    @shopping_before.setter
    def shopping_before(self, shopping_before: bool):
        """Sets the shopping_before of this PlanItem.


        :param shopping_before: The shopping_before of this PlanItem.
        :type shopping_before: bool
        """

        self._shopping_before = shopping_before

    @property
    def name(self) -> str:
        """Gets the name of this PlanItem.


        :return: The name of this PlanItem.
        :rtype: str
        """
        return self._name

    @name.setter
    def name(self, name: str):
        """Sets the name of this PlanItem.


        :param name: The name of this PlanItem.
        :type name: str
        """

        self._name = name

    @property
    def recipes(self) -> List[object]:
        """Gets the recipes of this PlanItem.


        :return: The recipes of this PlanItem.
        :rtype: List[object]
        """
        return self._recipes

    @recipes.setter
    def recipes(self, recipes: List[object]):
        """Sets the recipes of this PlanItem.


        :param recipes: The recipes of this PlanItem.
        :type recipes: List[object]
        """

        self._recipes = recipes
