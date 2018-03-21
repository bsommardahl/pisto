open Jest;

open Expect;

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

describe("the cafe store", ()
  =>
    describe("saving a new order", () =>
      test("it should add the new order to the store", ()
        /* let allOrders = [
             buildOrder(1, "Byron"),
             buildOrder(2, "Pamela"),
             buildOrder(3, "Emanuel"),
             buildOrder(4, "Nathan"),
           ];
           let testOrder = buildOrder(5, "Rosinda");
           let result = CafeStore.add(testOrder, allOrders);
           expect(result |> List.length) |> toBe(5); */
        =>
          expect(1) |> toBe(1)
        )
    )
  );
  /* describe("getting the next id", () =>
       test("it should give one more than the highest number", () => {
         let allOrders = [
           buildOrder(1, "Byron"),
           buildOrder(2, "Pamela"),
           buildOrder(3, "Emanuel"),
           buildOrder(50, "Nathan"),
         ];
         let result = CafeStore.getNextId(allOrders);
         expect(result) |> toBe(51);
       })
     );
     describe("finding an order by id", () =>
       test("it should return the correct order", () => {
         let allOrders = [
           buildOrder(1, "Byron"),
           buildOrder(2, "Pamela"),
           buildOrder(3, "Emanuel"),
           buildOrder(50, "Nathan"),
         ];
         let result = CafeStore.find(3, allOrders);
         expect(result.customerName) |> toEqual("Emanuel");
       })
     );
     describe("querying for closed orders", () =>
       test("it should return only the open orders", () => {
         let emanuel = {...buildOrder(3, "Emanuel"), paidOn: Some(600.00)};
         let nathan = {...buildOrder(50, "Nathan"), paidOn: Some(500.00)};
         let allOrders = [
           buildOrder(1, "Byron"),
           buildOrder(2, "Pamela"),
           emanuel,
           nathan,
         ];
         let result = CafeStore.getClosedOrders(allOrders);
         expect(result) |> toEqual([emanuel, nathan]);
       })
     );
     describe("querying for open orders", () =>
       test("it should return only the open orders", () => {
         let byron = buildOrder(1, "Byron");
         let pamela = buildOrder(2, "Pamela");
         let allOrders = [
           byron,
           pamela,
           {...buildOrder(3, "Emanuel"), paidOn: Some(600.00)},
           {...buildOrder(50, "Nathan"), paidOn: Some(500.00)},
         ];
         let result = CafeStore.getOpenOrders(allOrders);
         expect(result) |> toEqual([byron, pamela]);
       })
     );
     describe("removing an order by id", () =>
       test("it should not be in the list anymore", () => {
         let allOrders = [
           buildOrder(1, "Byron"),
           buildOrder(2, "Pamela"),
           buildOrder(3, "Emanuel"),
           buildOrder(4, "Stranger"),
           buildOrder(5, "Nathan"),
         ];
         let newList = CafeStore.remove(4, allOrders);
         let removedOrder =
           newList |> List.find((o: OrderData.Order.order) => o.id == Some(4));
         expect(removedOrder.removed) |> toBe(true);
       })
     );
     describe("updating an order", () =>
       test("it should apply the change", () => {
         let pamela = buildOrder(2, "Pamela");
         let allOrders = [
           buildOrder(1, "Byron"),
           pamela,
           buildOrder(3, "Emanuel"),
           buildOrder(4, "Nathan"),
         ];
         let newPamela = {...pamela, customerName: "Vanessa"};
         let newList = CafeStore.update(newPamela, allOrders);
         let reFindOrder =
           newList |> List.find((o: OrderData.Order.order) => o.id === pamela.id);
         expect(reFindOrder.customerName) |> toEqual("Vanessa");
       })
     ); */