type t = {
  code: string,
  name: string,
  tags: list(string),
  suggestedPrice: int,
  taxCalculation: Tax.taxCalculationMethod,
};

let getProducts = () => [
  {
    code: "Americano15",
    name: "Americano 15",
    tags: ["Cafes"],
    suggestedPrice: 1500,
    taxCalculation: Exempt,
  },
  {
    code: "Americano20",
    name: "Americano 20",
    tags: ["Cafes", "Drinks"],
    suggestedPrice: 2000,
    taxCalculation: Exempt,
  },
  {
    code: "Americano20",
    name: "Americano 20",
    tags: ["Cafes"],
    suggestedPrice: 2000,
    taxCalculation: Exempt,
  },
  {
    code: "PapasFritas",
    name: "Papas Fritas",
    tags: ["Burgers", "Sides"],
    suggestedPrice: 2000,
    taxCalculation: TotalFirst(15),
  },
  {
    code: "Aloja",
    name: "Aloja Burger",
    tags: ["Burgers"],
    suggestedPrice: 8000,
    taxCalculation: SubTotalFirst(15),
  },
  {
    code: "PizzaPep",
    name: "Pizza Peperoni",
    tags: ["Pizza"],
    suggestedPrice: 20000,
    taxCalculation: TotalFirst(15),
  },
  {
    code: "PizzaCheese",
    name: "Pizza Queso",
    tags: ["Pizza"],
    suggestedPrice: 20000,
    taxCalculation: TotalFirst(15),
  },
  {
    code: "PizzaAloha",
    name: "Pizza Aloja",
    tags: ["Pizza"],
    suggestedPrice: 20000,
    taxCalculation: TotalFirst(15),
  },
];

let getTags = (products: list(t)) : list(string) => {
  let tagArrArr = products |> List.map(p => p.tags);
  let allTags = tagArrArr |> List.flatten;
  let uniqueTags =
    allTags |> List.sort_uniq((tag1, tag2) => String.compare(tag1, tag2));
  uniqueTags;
};

let filterProducts = (tag: string, products: list(t)) : list(t) =>
  products |> List.filter(p => p.tags |> List.exists(t => t === tag));