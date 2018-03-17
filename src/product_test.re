open Jest;

open Expect;

let buildProduct = (name: string, tags: list(string)) : Product.t => {
  id: "123",
  name,
  tags,
  suggestedPrice: 10.00,
};

describe("product data functions", () => {
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
      expect(result |> List.length) |> toBe(4);
      expect(result) |> toEqual(["a", "b", "c", "d"]);
    })
  );
});