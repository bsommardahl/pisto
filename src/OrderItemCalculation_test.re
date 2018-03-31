open Jest;

open Expect;

open OrderData;

open OrderItemCalculation;

let buildOrderItem = (product: Product.t) : Order.orderItem => {
  sku: product.sku,
  name: product.name,
  suggestedPrice: product.suggestedPrice,
  addedOn: Js.Date.now(),
  salePrice: product.suggestedPrice,
  taxCalculation: product.taxCalculation,
};

let buildNewOrder =
    (customerName: string, orderItems, discounts)
    : Order.orderVm => {
  id: None,
  customerName,
  orderItems,
  createdOn: Js.Date.now(),
  discounts,
  paidOn: None,
  amountPaid: None,
  paymentTakenBy: None,
  lastUpdated: None,
  removed: false,
};

describe("order item calculation", () =>
  describe("when calculating order item totals", () => {
    describe("with several items", () => {
      let orderItems: list(OrderData.Order.orderItem) = [
        {
          sku: "PizzaPep",
          name: "Pizza Peperoni",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
        {
          sku: "PizzaCheese",
          name: "Pizza Queso",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
        {
          sku: "PizzaAloha",
          name: "Pizza Aloja",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
      ];
      test("the tax and the subtotal should equal the total", () => {
        let totals = orderItems |> OrderItemCalculation.getTotals([]);
        expect(totals.subTotal) |> toEqual(52173) |> ignore;
        expect(totals.tax) |> toEqual(7827) |> ignore;
        expect(totals.subTotal + totals.tax) |> toEqual(totals.total);
      });
    });
    describe("with several items and a discount", () => {
      let orderItems: list(OrderData.Order.orderItem) = [
        {
          sku: "PizzaPep",
          name: "Pizza Peperoni",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
        {
          sku: "PizzaCheese",
          name: "Pizza Queso",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
        {
          sku: "PizzaAloha",
          name: "Pizza Aloja",
          suggestedPrice: 20000,
          taxCalculation: TotalFirst(15),
          addedOn: 1.1,
          salePrice: 20000,
        },
      ];
      let totals =
        orderItems
        |> OrderItemCalculation.getTotals([
             {id: "test", name: "test", percent: 10},
           ]);
      test("the discount should be 10% of the subtotal", () =>
        expect(totals.discounts) |> toEqual(521)
      );
      test("the discount should be applied to the subtotal", () =>
        expect(totals.subTotal) |> toEqual(52173 - 521)
      );
      test("the tax should be affected by the discount", () =>
        expect(totals.tax) |> toEqual((52173 - 521) * 1500)
      );
    });
    describe("with exempt tax calculation", () => {
      let orderItem: Order.orderItem = {
        sku: "some",
        name: "some",
        suggestedPrice: 0,
        addedOn: 0.0,
        salePrice: 100,
        taxCalculation: Tax.Exempt,
      };
      test("it should return zero tax", () => {
        let totals = exemptCalculator(0, orderItem);
        expect(totals)
        |> toEqual({subTotal: 100, discounts: 0, tax: 0, total: 100});
      });
    });
    describe("with total first tax calculation", () => {
      let orderItem: Order.orderItem = {
        sku: "some",
        name: "some",
        suggestedPrice: 0,
        addedOn: 0.0,
        salePrice: 100,
        taxCalculation: Tax.TotalFirst(15),
      };
      test("it should return the sale price as the total", () => {
        let totals = totalFirstCalculator(15, orderItem);
        expect(totals)
        |> toEqual({subTotal: 86, discounts: 0, tax: 14, total: 100});
      });
    });
    describe("with subtotal first tax calculation", () => {
      let orderItem: Order.orderItem = {
        sku: "some",
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
          expect(totals)
          |> toEqual({subTotal: 100, discounts: 0, tax: 15, total: 115});
        },
      );
    });
  })
);