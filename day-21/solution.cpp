#include "aoclib.hpp"

struct Ingredient;
std::ostream &operator<<(std::ostream &output, const Ingredient &i);

/**
 * Allergen
 */

struct Allergen {
  std::string name;
  std::optional<Ingredient *> foundIn;

  Allergen(std::string name, std::optional<Ingredient *> foundIn) : name(name), foundIn(foundIn) {}
};

std::ostream &operator<<(std::ostream &output, const Allergen &a) {
  output << a.name;
  return output;
}

/**
 * Ingredient
 */

struct Ingredient {
  std::string name;
  std::optional<Allergen *> allergen;

  Ingredient(std::string name, std::optional<Allergen *> allergen) : name(name), allergen(allergen) {}
};

std::ostream &operator<<(std::ostream &output, const Ingredient &i) {
  output << i.name;
  if (i.allergen.has_value()) output << " (" << *i.allergen.value() << ")";
  return output;
}

/**
 * Dish
 */

class Dish {
 private:
  std::set<Ingredient *> ingredients;
  std::set<Allergen *> allergens;

 public:
  Dish(std::set<Ingredient *> ingredients, std::set<Allergen *> allergens)
      : ingredients(ingredients), allergens(allergens) {}

  std::set<Ingredient *> getIngredients() const { return this->ingredients; }
  std::set<Allergen *> getAllergens() const { return this->allergens; }

  void refreshAllergens() {
    for (auto &ingred : this->ingredients) {
      if (ingred->allergen.has_value()) {
        this->allergens.insert(ingred->allergen.value());
      }
    }
  }
};

std::ostream &operator<<(std::ostream &output, const Dish &d) {
  auto ingredients = d.getIngredients();
  for (auto i : ingredients) output << *i << ", ";

  auto allergens = d.getAllergens();
  output << "(";
  for (auto a : allergens) output << *a << ", ";
  output << ")";

  return output;
}

/**
 * Parse input
 */

Dish parseInput(std::string &line, std::vector<std::unique_ptr<Ingredient>> &allIngredients,
                std::vector<std::unique_ptr<Allergen>> &allAllergens) {
  std::vector<std::string> splitValues;
  boost::split(splitValues, line, boost::is_any_of("("));
  std::string ingredientsStr = splitValues[0];
  std::string allergensStr = splitValues[1];
  allergensStr = allergensStr.substr(9, allergensStr.size() - 10);
  splitValues.clear();

  // Parse ingredients
  std::set<Ingredient *> ingredients;
  boost::split(splitValues, ingredientsStr, boost::is_any_of(" "));
  for (auto &ingName : splitValues) {
    if (ingName == "") continue;

    auto predicate = [&ingName](std::unique_ptr<Ingredient> &i) { return i->name == ingName; };
    auto search = std::find_if(allIngredients.begin(), allIngredients.end(), predicate);

    if (search == allIngredients.end()) {
      allIngredients.push_back(std::make_unique<Ingredient>(ingName, std::nullopt));
      search = allIngredients.end() - 1;
    }

    ingredients.insert((*search).get());
  }

  // Parse allergens
  splitValues.clear();

  std::set<Allergen *> allergens;
  boost::split(splitValues, allergensStr, boost::is_any_of(", "));
  for (auto &allName : splitValues) {
    if (allName == "") continue;

    auto predicate = [&allName](std::unique_ptr<Allergen> &i) { return i->name == allName; };
    auto search = std::find_if(allAllergens.begin(), allAllergens.end(), predicate);

    if (search == allAllergens.end()) {
      allAllergens.push_back(std::make_unique<Allergen>(allName, std::nullopt));
      search = allAllergens.end() - 1;
    }

    allergens.insert((*search).get());
  }

  return Dish(ingredients, allergens);
}

/**
 * Solve
 */

// Find intersection of all dishes that contain given allergen
std::set<Ingredient *> findPossibleIngredientsWithAllergen(std::vector<Dish> &dishes,
                                                           std::vector<std::unique_ptr<Ingredient>> &allIngredients,
                                                           Allergen *allergen) {
  std::set<Ingredient *> possibleIngredients;
  for (auto &ingred : allIngredients) {
    // Ignore ingredients that were already matched
    if (!ingred->allergen.has_value()) {
      possibleIngredients.insert(ingred.get());
    }
  }

  for (auto &dish : dishes) {
    std::set<Allergen *> dishAllergens = dish.getAllergens();
    auto search = std::find(dishAllergens.begin(), dishAllergens.end(), allergen);

    if (search != dishAllergens.end()) {
      std::set<Ingredient *> dishIngredients = dish.getIngredients();
      std::set<Ingredient *> tmp;
      std::set_intersection(possibleIngredients.begin(), possibleIngredients.end(), dishIngredients.begin(),
                            dishIngredients.end(), std::inserter(tmp, tmp.end()));
      possibleIngredients = tmp;
    }
  }

  return possibleIngredients;
}

void findAllAllergens(std::vector<Dish> &dishes, std::vector<std::unique_ptr<Ingredient>> &allIngredients,
                      std::vector<std::unique_ptr<Allergen>> &allAllergens) {
  unsigned int numAllergensMatched = 0;
  while (numAllergensMatched < allAllergens.size()) {
    for (auto &allergen : allAllergens) {
      if (allergen->foundIn.has_value()) continue;

      std::set<Ingredient *> possibleIngredients =
          findPossibleIngredientsWithAllergen(dishes, allIngredients, allergen.get());

      if (possibleIngredients.size() == 1) {
        Ingredient *ingredient = *possibleIngredients.begin();
        ingredient->allergen = allergen.get();
        allergen->foundIn = ingredient;
        ++numAllergensMatched;

        for (auto &dish : dishes) {
          dish.refreshAllergens();
        }
      }
    }
  }
}

void part1(std::vector<Dish> &dishes, std::vector<std::unique_ptr<Ingredient>> &allIngredients,
           std::vector<std::unique_ptr<Allergen>> &allAllergens) {
  findAllAllergens(dishes, allIngredients, allAllergens);

  unsigned int count = 0;
  for (auto &dish : dishes) {
    for (auto &ingred : dish.getIngredients()) {
      if (!ingred->allergen.has_value()) ++count;
    }
  }
  std::cout << count << std::endl;
}

void part2(std::vector<std::unique_ptr<Allergen>> &allAllergens) {
  struct {
    bool operator()(Allergen *a, Allergen *b) const { return a->name < b->name; }
  } compare;

  std::sort(allAllergens.begin(), allAllergens.end(),
            [](std::unique_ptr<Allergen> &a, std::unique_ptr<Allergen> &b) { return a->name < b->name; });

  std::string canonicalDangerousIngredientList = "";
  for (auto &a : allAllergens) {
    canonicalDangerousIngredientList += a->foundIn.value()->name + ",";
  }
  canonicalDangerousIngredientList.erase(canonicalDangerousIngredientList.size() - 1);
  std::cout << canonicalDangerousIngredientList << std::endl;
}

int main() {
  const std::string filename = "../day-21/input.txt";
  auto input = aoc::readStringInput(filename);

  std::vector<std::unique_ptr<Ingredient>> allIngredients;
  std::vector<std::unique_ptr<Allergen>> allAllergens;

  std::vector<Dish> dishes;
  for (auto &line : input) {
    dishes.push_back(parseInput(line, allIngredients, allAllergens));
  }

  part1(dishes, allIngredients, allAllergens);
  part2(allAllergens);

  return 0;
}
