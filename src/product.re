type t = {
  id: string,
  name: string,
  tags: list(string),
  suggestedPrice: float,
};

let getProducts = () => [
  {
    id: "Americano15",
    name: "Americano 15",
    tags: ["Cafes"],
    suggestedPrice: 15.00,
  },
  {
    id: "Americano20",
    name: "Americano 20",
    tags: ["Cafes", "Drinks"],
    suggestedPrice: 20.00,
  },
  {
    id: "Americano20",
    name: "Americano 20",
    tags: ["Cafes"],
    suggestedPrice: 20.00,
  },
  {
    id: "PapasFritas",
    name: "Papas Fritas",
    tags: ["Burgers", "Sides"],
    suggestedPrice: 20.00,
  },
  {
    id: "Aloja",
    name: "Aloja Burger",
    tags: ["Burgers"],
    suggestedPrice: 80.00,
  },
  {
    id: "PizzaPep",
    name: "Pizza Peperoni",
    tags: ["Pizza"],
    suggestedPrice: 200.00,
  },
  {
    id: "PizzaCheese",
    name: "Pizza Queso",
    tags: ["Pizza"],
    suggestedPrice: 200.00,
  },
  {
    id: "PizzaAloha",
    name: "Pizza Aloja",
    tags: ["Pizza"],
    suggestedPrice: 200.00,
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