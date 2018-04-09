open Jest;

open Expect;

let buildProduct = (name: string, tags: list(string)) : Product.t => {
  id: "123",
  sku: "123",
  name,
  tags,
  suggestedPrice: 1000,
  taxCalculation: Exempt,
};

describe("product related utilities", () =>
  describe("js obj mapping", () =>
    test("it should be able to map an existing product from js", () => {
      let js = {
        "_id": "982f1879-c98c-441a-8149-ba6282e8db58",
        "_rev": "1-cb789488740c442cba15653323c005bb",
        "sku": "fasdf",
        "name": "fasdf",
        "suggestedPrice": 3543534,
        "tags": ["fsdafsda"],
        "taxCalculation": "exempt|",
      };
      let product: Product.t = js |> Product.mapFromJs;
      expect(product.id) |> toEqual(js##_id);
    })
  )
);
/* describe("product data functions", () => {
     describe("filtering products by tag", () =>
       test("it should return only relevant products", () => {
         let result =
           Product.filterProducts(
             "a",
             [
               buildProduct("plate", ["a"]),
               buildProduct("platter", ["b"]),
               buildProduct("bowl", ["a"]),
             ],
           );
         expect(result)
         |> toEqual([
             buildProduct("plate", ["a"]),
             buildProduct("bowl", ["a"]),
           ]);
       })
     );
     describe("getting only unique tags", (_) =>
       test("it should return a list of unique tags", (_) => {
         let result =
           Product.getTags([
             buildProduct("coffee", ["a"]),
             buildProduct("latte", ["b"]),
             buildProduct("macchiato", ["a"]),
             buildProduct("white", ["c", "d", "a"]),
             buildProduct("black", ["b"]),
             buildProduct("au lait", ["a"]),
           ]);
         expect(result |> List.length) |> toBe(4) |> ignore;
         expect(result) |> toEqual(["a", "b", "c", "d"]);
       })
     );
   }); */