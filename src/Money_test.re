open Jest;

open Expect;

describe("Money functions", () => {
  describe("when converting a mixed number to money display", () =>
    test("it should show the amount with decimal places", () => {
      let amount = 123456;
      let display = amount |> Money.toDisplay;
      expect(display) |> toEqual("L.1234.56");
    })
  );
  describe("when converting a closed number to money display", () =>
    test("it should show the amount with decimal places", () => {
      let amount = 1000;
      let display = amount |> Money.toDisplay;
      expect(display) |> toEqual("L.10.00");
    })
  );
  describe("when converting a valid money display string to int", () =>
    test("it should return the correct int", () => {
      let display = "L.10.00";
      let amount = display |> Money.toT;
      expect(amount) |> toEqual(1000);
    })
  );
  describe("when converting an invalid money display string to int", () =>
    test("it should return the correct int", () => {
      let display = "N.10.000";
      let amount = display |> Money.toT;
      expect(amount) |> toEqual(1000);
    })
  );
});