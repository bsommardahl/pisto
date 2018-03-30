type t = {
  id: string,
  sku: string,
  name: string,
  tags: list(string),
  suggestedPrice: int,
  taxCalculation: Tax.taxCalculationMethod,
};

module NewProduct = {
  type t = {
    sku: string,
    name: string,
    tags: list(string),
    suggestedPrice: int,
    taxCalculation: Tax.taxCalculationMethod,
  };
  let mapToJs = (prod: t) => {
    "sku": prod.sku,
    "name": prod.name,
    "suggestedPrice": prod.suggestedPrice,
    "tags": prod.tags |> Array.of_list,
    "taxCalculation": prod.taxCalculation |> Tax.Calculation.toDelimitedString,
  };
};

let mapFromJs = prodJs : t => {
  id: prodJs##_id,
  sku: prodJs##sku,
  name: prodJs##name,
  suggestedPrice: prodJs##suggestedPrice,
  tags: prodJs##tags,
  taxCalculation: prodJs##taxCalculation |> Tax.Calculation.toMethod,
};

let mapToJsWithRev = (id: string, rev: option(string), prod: t) => {
  "_id": id,
  "_rev": Js.Null.fromOption(rev),
  "sku": prod.sku,
  "name": prod.name,
  "suggestedPrice": prod.suggestedPrice,
  "tags": prod.tags,
  "taxCalculation": prod.taxCalculation |> Tax.Calculation.toDelimitedString,
};

let mapToJs = (prod: t) => prod |> mapToJsWithRev(prod.id, None);

let getTags = (products: list(t)) : list(string) => {
  let tagArrArr = products |> List.map(p => p.tags);
  let allTags = tagArrArr |> List.flatten;
  let uniqueTags =
    allTags |> List.sort_uniq((tag1, tag2) => String.compare(tag1, tag2));
  uniqueTags;
};

let filterProducts = (tag: string, products: list(t)) : list(t) =>
  products |> List.filter(p => p.tags |> List.exists(t => t === tag));