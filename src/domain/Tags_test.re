open Jest;

open Expect;

describe("Tag utilities", () =>
  describe("converting tags", () => {
    test("from valid string to list", () =>
      expect("mickey,mouse,donald,duck" |> Tags.toList)
      |> toEqual(["mickey", "mouse", "donald", "duck"])
    );
    test("from invalid string to list", () =>
      expect("mickey,,mouse|donald,duck," |> Tags.toList)
      |> toEqual(["mickey", "mouse|donald", "duck"])
    );
    test("from string with extra spaces to list", () =>
      expect("mickey,  mouse,donald  ,duck," |> Tags.toList)
      |> toEqual(["mickey", "mouse", "donald", "duck"])
    );
    test("from empty string to empty list", () =>
      expect("" |> Tags.toList) |> toEqual([])
    );
  })
);