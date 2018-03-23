open Jest;

open Expect;

open OrderData;

open Pouchdb;

let buildOrder = (id: int, customerName: string) : OrderData.Order.order => {
  id: Some(id),
  customerName,
  orderItems: [],
  createdOn: 123.0,
  paidOn: None,
  paymentTakenBy: None,
  amountPaid: None,
  removed: false,
  lastUpdated: None,
};

describe("the cafe store", () =>
  describe("saving a new order", () =>
    test("it should add the new order to the store", () => {
      let testOrder = buildOrder(5, "Rosinda");
      let result = CafeStore.add(testOrder, connection);
      expect(result |> List.length) |> toBe(5);
      expect(1) |> toBe(2);
    })
  )
);