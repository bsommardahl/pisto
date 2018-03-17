open Jest;

open Expect;

describe("The Cafe Router", () =>
  describe("when going to the web root", () =>
    test("it should go to the home view", () =>
      expect(1 + 1) |> toBe(2)
    )
  )
);