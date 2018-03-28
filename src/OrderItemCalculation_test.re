open Jest;

open Expect;

open OrderData;

open OrderItemCalculation;

describe("order item calculation", () =>
  describe("when calculating order item totals", () => {
    describe("with exempt tax calculation", () => {
      let orderItem: Order.orderItem = {
        productCode: "some",
        name: "some",
        suggestedPrice: 0,
        addedOn: 0.0,
        salePrice: 100,
        taxCalculation: Tax.Exempt,
      };
      test("it should return zero tax", () => {
        let totals = exemptCalculator(0, orderItem);
        expect(totals) |> toEqual({subTotal: 100, tax: 0, total: 100});
      });
    });
    describe("with total first tax calculation", () => {
      let orderItem: Order.orderItem = {
        productCode: "some",
        name: "some",
        suggestedPrice: 0,
        addedOn: 0.0,
        salePrice: 100,
        taxCalculation: Tax.TotalFirst(15),
      };
      test("it should return the sale price as the total", () => {
        let totals = totalFirstCalculator(15, orderItem);
        expect(totals) |> toEqual({subTotal: 86, tax: 14, total: 100});
      });
    });
    describe("with subtotal first tax calculation", () => {
      let orderItem: Order.orderItem = {
        productCode: "some",
        name: "some",
        suggestedPrice: 0,
        addedOn: 0.0,
        salePrice: 100,
        taxCalculation: Tax.TotalFirst(15),
      };
      test(
        "it should return the sale price as the subtotal and add the tax for the total",
        () => {
          let totals = subTotalFirstCalculator(15, orderItem);
          expect(totals) |> toEqual({subTotal: 100, tax: 15, total: 115});
        },
      );
    });
  })
);